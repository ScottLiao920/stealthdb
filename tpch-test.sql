drop extension encdb cascade;
create extension encdb;
create server cstore_server foreign data wrapper cstore_fdw;
create foreign table customer_enc(
    c_custkey_enc enc_int4,
    c_name varchar(25),
    c_address varchar(40),
    c_nation_key_enc enc_int4,
    c_phone char(15),
    c_acctbal numeric(15, 2),
    c_mktsegment char(10),
    c_comment varchar(117)
    )
    server cstore_server options (compression 'lz4');
create foreign table lineitem_enc(
    l_orderkey_enc enc_int4 not null,
    l_partkey_enc enc_int4 not null,
    l_suppkey_enc enc_int4 not null,
    l_linenumber_enc enc_int4 not null,
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
    server cstore_server options (compression 'lz4');
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

COPY (select pg_enc_int4_encrypt(l_orderkey),
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
COPY customer_enc from '/home/scott/Projects/stealthdb/tpch/enc_customer.csv' With CSV DELIMITER ',' HEADER;
COPY lineitem_enc from '/home/scott/Projects/stealthdb/tpch/enc_lineitem.csv' With CSV DELIMITER ',' HEADER;