xquery version "1.0-ml";

module namespace bucket = "http://marklogic.com/bucket";

import module namespace search = "http://marklogic.com/appservices/search" at "/MarkLogic/appservices/search/search.xqy";

declare option xdmp:mapping "false";

declare function bucket:parse(
  $query-elem as element(),
  $options as element(search:options))
as schema-element(cts:query)
{
  ()
};

declare function bucket:start(
  $constraint as element(search:constraint),
  $query as cts:query?,
  $facet-options as xs:string*,
  $quality-weight as xs:double?,
  $forests as xs:unsignedLong*)
as item()*
{
  let $buckets := $constraint/search:custom/search:bucket
  for $bucket in $buckets
  return
    element bucket {
      attribute count {
        xdmp:estimate(
          cts:search(
            fn:doc(),
            cts:and-query((
              cts:element-range-query(xs:QName("lo"), "<=", xs:int($bucket/@hi)),
              cts:element-range-query(xs:QName("hi"), ">=", xs:int($bucket/@lo))
            ))))
        },
      $bucket/@lo,
      $bucket/@hi
    }
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
    for $range in $start
    return
    element search:facet-value {
      attribute name { $range/@lo || "-" || $range/@hi },
      $range/@count
    }
  }
};
