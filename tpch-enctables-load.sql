drop extension encdb cascade;
create extension encdb;
create server cstore_server foreign data wrapper cstore_fdw;

create foreign table customer_enc(
    c_custkey enc_int4,
    c_name varchar(25),
    c_address varchar(40),
    c_nation_key enc_int4,
    c_phone char(15),
    c_acctbal numeric(15, 2),
    c_mktsegment char(10),
    c_comment varchar(117)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table lineitem_enc(
    l_orderkey enc_int4 not null,
    l_partkey enc_int4 not null,
    l_suppkey enc_int4 not null,
    l_linenumber enc_int4 not null,
    l_quantity numeric(15, 2) not null,
    l_extendedprice numeric(15, 2) not null,
    l_discount numeric(15, 2) not null,
    l_tax numeric(15, 2) not null,
    l_returnflag char not null,
    l_linestatus char not null,
    l_shipdate date not null,
    l_commitdate date not null,
    l_receiptdate date not null,
    l_shipinstruct char(25) not null,
    l_shipmode char(10) not null,
    l_comment varchar(44) not null
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');

create foreign table nation_enc(
    n_nationkey enc_int4,
    n_name char(25),
    n_regionkey enc_int4,
    n_comment varchar(152)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');

create foreign table orders_enc(
    o_orderkey enc_int4,
    o_custkey enc_int4,
    o_orderstatus char,
    o_totalprice numeric(15, 2),
    o_orderdate date,
    o_orderpriority char(15),
    o_clerk char(15),
    o_shippriority integer,
    o_comment varchar(79)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');



create foreign table part_enc(
    p_partkey enc_int4,
    p_name varchar(55),
    p_mfgr char(25),
    p_brand char(10),
    p_type varchar(25),
    p_size integer,
    p_container char(10),
    p_retailprice numeric(15, 2),
    p_comment varchar(23)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');

create foreign table partsupp_enc(
    ps_partkey enc_int4,
    ps_suppkey enc_int4,
    ps_availqty integer,
    ps_supplycost numeric(15, 2),
    ps_comment varchar(199)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');

create foreign table region_enc(
    r_regionkey enc_int4,
    r_name char(25),
    r_comment varchar(152)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000');

create foreign table supplier_enc(
    s_suppkey enc_int4,
    s_name char(25),
    s_address varchar(40),
    s_nationkey integer,
    s_phone char(15),
    s_acctbal numeric(15, 2),
    s_comment varchar(101)
    )
    server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

-- COPY customer_enc from '/home/scott/Projects/stealthdb/tpch/customer.csv' With CSV DELIMITER ',' HEADER;
-- COPY lineitem_enc from '/home/scott/Projects/stealthdb/tpch/lineitem.csv' With CSV DELIMITER ',' HEADER;
-- COPY nation_enc from '/home/scott/Projects/stealthdb/tpch/nation.csv' With CSV DELIMITER ',' HEADER;
-- COPY orders_enc from '/home/scott/Projects/stealthdb/tpch/orders.csv' With CSV DELIMITER ',' HEADER;
-- COPY part_enc from '/home/scott/Projects/stealthdb/tpch/part.csv' With CSV DELIMITER ',' HEADER;
-- COPY partsupp_enc from '/home/scott/Projects/stealthdb/tpch/partsupp.csv' With CSV DELIMITER ',' HEADER;
-- COPY region_enc from '/home/scott/Projects/stealthdb/tpch/region.csv' With CSV DELIMITER ',' HEADER;
COPY supplier_enc from '/home/scott/Projects/stealthdb/tpch/supplier.csv' With CSV DELIMITER ',' HEADER;

create foreign table supplier_enc_vec(
    s_suppkey enc_int4,
    s_name char(25),
    s_address varchar(40),
    s_nationkey integer,
    s_phone char(15),
    s_acctbal numeric(15, 2),
    s_comment varchar(101)
    )
    server cstore_server options (compression 'lz4', block_row_count '1000', stripe_row_count '1000');
COPY supplier_enc_vec from '/home/scott/Projects/stealthdb/tpch/enc_supplier.csv' With CSV DELIMITER ',' HEADER;

drop table supplier_enc_pg;
create table supplier_enc_pg
(
    s_suppkey   enc_int4,
    s_name      char(25),
    s_address   varchar(40),
    s_nationkey integer,
    s_phone     char(15),
    s_acctbal   numeric(15, 2),
    s_comment   varchar(101)
);
COPY supplier_enc_pg from '/home/scott/Projects/stealthdb/tpch/enc_supplier.csv' With CSV DELIMITER ',' HEADER;
