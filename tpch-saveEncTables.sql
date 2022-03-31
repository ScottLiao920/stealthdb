create extension encdb;

COPY (select pg_enc_int4_encrypt(c_custkey),
             c_name,
             c_address,
             pg_enc_int4_encrypt(c_nationkey),
             c_phone,
             c_acctbal,
             c_mktsegment,
             c_comment
      from customer)
    TO '/home/scott/Projects/stealthdb/tpch/enc_customer.csv' With CSV DELIMITER ',' HEADER;

COPY (
    select pg_enc_int4_encrypt(l_orderkey),
           pg_enc_int4_encrypt(l_partkey),
           pg_enc_int4_encrypt(l_suppkey),
           pg_enc_int4_encrypt(l_linenumber),
           l_quantity,
           l_extendedprice,
           l_discount,
           l_tax,
           l_returnflag,
           l_linestatus,
           l_shipdate,
           l_commitdate,
           l_receiptdate,
           l_shipinstruct,
           l_shipmode,
           l_comment
    from lineitem)
    TO '/home/scott/Projects/stealthdb/tpch/enc_lineitem.csv' With CSV DELIMITER ',' HEADER;


COPY (
    SELECT pg_enc_int4_encrypt(n_nationkey),
           n_name,
           pg_enc_int4_encrypt(n_regionkey),
           n_comment
    FROM nation
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_nation.csv' With CSV DELIMITER ',' HEADER;


COPY (
    SELECT pg_enc_int4_encrypt(o_orderkey),
           pg_enc_int4_encrypt(o_custkey),
           o_orderstatus,
           o_totalprice,
           o_orderdate,
           o_orderpriority,
           o_clerk,
           o_shippriority,
           o_comment
    FROM orders
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_orders.csv' With CSV DELIMITER ',' HEADER;

COPY (
    SELECT pg_enc_int4_encrypt(p_partkey),
           p_name,
           p_mfgr,
           p_brand,
           p_type,
           p_size,
           p_container,
           p_retailprice,
           p_comment
    FROM part
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_part.csv' With CSV DELIMITER ',' HEADER;


COPY (
    SELECT pg_enc_int4_encrypt(ps_partkey),
           pg_enc_int4_encrypt(ps_suppkey),
           ps_availqty,
           ps_supplycost,
           ps_comment
    FROM partsupp
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_partsupp.csv' With CSV DELIMITER ',' HEADER;


COPY (
    SELECT pg_enc_int4_encrypt(r_regionkey),
           r_name,
           r_comment
    FROM region
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_region.csv' With CSV DELIMITER ',' HEADER;

COPY (
    SELECT pg_enc_int4_encrypt(s_suppkey),
           s_name,
           s_address,
           s_nationkey,
           s_phone,
           s_acctbal,
           s_comment
    FROM supplier
    ) TO '/home/scott/Projects/stealthdb/tpch/enc_supplier.csv' With CSV DELIMITER ',' HEADER;

COPY (

    SELECT
        *
    FROM
        customer
    ) TO '/home/scott/Projects/stealthdb/tpch/customer.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        lineitem
    ) TO '/home/scott/Projects/stealthdb/tpch/lineitem.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        nation
    ) TO '/home/scott/Projects/stealthdb/tpch/nation.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        orders
    ) TO '/home/scott/Projects/stealthdb/tpch/orders.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        part
    ) TO '/home/scott/Projects/stealthdb/tpch/part.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        partsupp
    ) TO '/home/scott/Projects/stealthdb/tpch/partsupp.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        region
    ) TO '/home/scott/Projects/stealthdb/tpch/region.csv' With CSV DELIMITER ',' HEADER;
COPY (

    SELECT
        *
    FROM
        supplier
    ) TO '/home/scott/Projects/stealthdb/tpch/supplier.csv' With CSV DELIMITER ',' HEADER;
