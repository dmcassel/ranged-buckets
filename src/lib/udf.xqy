xquery version "1.0-ml";

module namespace bucket = "http://marklogic.com/bucket";

import module namespace search = "http://marklogic.com/appservices/search" at "/MarkLogic/appservices/search/search.xqy";

declare option xdmp:mapping "false";

declare function bucket:parse(
  $query-elem as element(),
  $options as element(search:options))
as schema-element(cts:query)
{
  <root>
  {
    let $bucket := fn:tokenize($query-elem/search:text, "-")
    return
      cts:and-query((
        cts:element-range-query(xs:QName("lo"), "<=", xs:int($bucket[2])),
        cts:element-range-query(xs:QName("hi"), ">=", xs:int($bucket[1]))
      ))
  }</root>/*
};

declare function bucket:start(
  $constraint as element(search:constraint),
  $query as cts:query?,
  $facet-options as xs:string*,
  $quality-weight as xs:double?,
  $forests as xs:unsignedLong*)
as item()*
{
  cts:aggregate(
    "davidcassel.rangedbucket/rangedbucket",
    "ranged-bucket",
    (
      cts:element-reference(xs:QName("lo"), "type=int"),
      cts:element-reference(xs:QName("hi"), "type=int")
    ),
    (
      (: The rangedbucket function expects a sequence with the number of
       : buckets followed by (lo, hi) values for each :)
      let $buckets := $constraint/search:custom/search:bucket
      return (
        fn:count($buckets),
        $buckets ! (./@lo, ./@hi)
      )
    )
  )
};

declare function bucket:finish(
  $start as item()*,
  $constraint as element(search:constraint),
  $query as cts:query?,
  $facet-options as xs:string*,
  $quality-weight as xs:double?,
  $forests as xs:unsignedLong*)
as element(search:facet)
{
  element search:facet {
    attribute name { $constraint/@name },
    let $buckets := $constraint/search:custom/search:bucket
    for $bucket at $i in $buckets
    let $key := $bucket/@lo || "-" || $bucket/@hi
    return
      element search:facet-value {
        attribute name { $key },
        attribute count { map:get($start, $key) }
      }
  }
};
