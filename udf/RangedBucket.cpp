#include <MarkLogic.h>
#include <string>

#ifdef _MSC_VER
#define PLUGIN_DLL __declspec(dllexport)
#else // !_MSC_VER
#define PLUGIN_DLL
#endif

using namespace marklogic;

////////////////////////////////////////////////////////////////////////////////

struct Bucket {
  int min, max;
  int count;
  Bucket() : min(0), max(0), count(0) {}
};

class RangedBucket : public AggregateUDF
{
public:
  Bucket *buckets;
  int bucketCount;

public:
  RangedBucket() {
    bucketCount = 0;
    buckets = 0;
  }
  RangedBucket(const RangedBucket &rhs);
  virtual ~RangedBucket();
  AggregateUDF* clone() const { return new RangedBucket(*this); }
  void close() { delete this; }

  void start(Sequence&, Reporter&);
  void finish(OutputSequence &os, Reporter &reporter);

  void map(TupleIterator &values, Reporter &reporter);
  void reduce(const AggregateUDF *_o, Reporter &reporter);

  void encode(Encoder &e, Reporter &reporter);
  void decode(Decoder &d, Reporter &reporter);
};

RangedBucket::RangedBucket(const RangedBucket &rhs)
{
  bucketCount = rhs.bucketCount;
  buckets = new Bucket[bucketCount];
  for (int i=0; i<bucketCount; i++) {
    buckets[i].min = rhs.buckets[i].min;
    buckets[i].max = rhs.buckets[i].max;
    buckets[i].count = rhs.buckets[i].count;
  }
}

RangedBucket::~RangedBucket()
{
  if (buckets)
    delete[] buckets;
}

void RangedBucket::start(Sequence &arg, Reporter &r)
{
  // std::string msg;
  if (arg.done()) {
    r.error("First argument must be bucket count -- not found.");
  } else {
    arg.value(bucketCount);
    // msg = "start: Bucket count is " + std::to_string(bucketCount);
    // r.log(Reporter::Info, msg.c_str());
    buckets = new Bucket[bucketCount];
    for (int i=0; i<bucketCount; i++) {
      if (arg.done()) {
        r.error("Did not find enough buckets");
      }

      arg.next();
      arg.value(buckets[i].min);
      arg.next();
      arg.value(buckets[i].max);
      buckets[i].count = 0;
    }
  }

}

void RangedBucket::finish(OutputSequence &os, Reporter &reporter)
{
  // std::string msg;

  // msg = "finish: bucketCount=" + std::to_string(bucketCount);
  // reporter.log(Reporter::Info, msg.c_str());

  os.startMap();

  for (int i=0; i<bucketCount; i++) {
    std::string key;
    key += std::to_string(buckets[i].min) + "-" + std::to_string(buckets[i].max);
    // msg = "finish: key=" + key + "; value=" + std::to_string(buckets[i].count);
    // reporter.log(Reporter::Info, msg.c_str());
    os.writeMapKey(key.c_str());
    os.writeValue(buckets[i].count);
  }

  os.endMap();
}

void RangedBucket::map(TupleIterator &values, Reporter &reporter)
{
  int lo, hi;
  // std::string msg;

  for(; !values.done(); values.next()) {
    if(!values.null(0)) {
      values.value(0, lo);
      values.value(1, hi);
      // msg = "map: lo=" + std::to_string(lo) + "; hi=" + std::to_string(hi);
      // reporter.log(Reporter::Info, msg.c_str());
      for (int i=0; i<bucketCount; i++) {
        if (lo <= buckets[i].max && hi >= buckets[i].min) {
          buckets[i].count += values.frequency();
          // reporter.log(Reporter::Info, ("Bucket " + std::to_string(buckets[i].min) + "-" + std::to_string(buckets[i].max) +
          //   " now has " + std::to_string(buckets[i].count)).c_str());
        }
      }
    }
  }
}

void RangedBucket::reduce(const AggregateUDF *_o, Reporter &reporter)
{
  const RangedBucket *o = (const RangedBucket*)_o;
  // std::string msg;
  for (int i=0; i<bucketCount; i++) {
    // msg = "reduce: " + std::to_string(buckets[i].min) + "-" + std::to_string(buckets[i].max) + "-> " +
    //   std::to_string(buckets[i].count) + " + " + std::to_string(o->buckets[i].count);
    // reporter.log(Reporter::Info, msg.c_str());
    buckets[i].count += o->buckets[i].count;
  }
}

void RangedBucket::encode(Encoder &e, Reporter &reporter)
{
  // reporter.log(Reporter::Info, "calling encode");
  e.encode(bucketCount);
  for (int i=0; i<bucketCount; i++) {
    e.encode(&buckets[i], sizeof(Bucket));
    // reporter.log(Reporter::Info,
    //   ("encode: " + std::to_string(buckets[i].min) + "-" + std::to_string(buckets[i].max) + "-> " + std::to_string(buckets[i].count)).c_str());
  }
}

void RangedBucket::decode(Decoder &d, Reporter &reporter)
{
  // reporter.log(Reporter::Info, "calling decode");
  d.decode(bucketCount);
  buckets = new Bucket[bucketCount];
  for (int i=0; i<bucketCount; i++) {
    d.decode(&buckets[i], sizeof(Bucket));
    // reporter.log(Reporter::Info,
    //   ("decode: " + std::to_string(buckets[i].min) + "-" + std::to_string(buckets[i].max) + "-> " + std::to_string(buckets[i].count)).c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////

extern "C" PLUGIN_DLL void
marklogicPlugin(Registry &r)
{
  r.version();
  r.registerAggregate<RangedBucket>("ranged-bucket");
}
