create foreign table NATION_vec ( N_NATIONKEY INTEGER NOT NULL,
    N_NAME CHAR(25) NOT NULL,
    N_REGIONKEY INTEGER NOT NULL,
    N_COMMENT VARCHAR(152)) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');
;

create foreign table REGION_vec ( R_REGIONKEY INTEGER NOT NULL,
    R_NAME CHAR(25) NOT NULL,
    R_COMMENT VARCHAR(152)) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table PART_vec ( P_PARTKEY INTEGER NOT NULL,
    P_NAME VARCHAR(55) NOT NULL,
    P_MFGR CHAR(25) NOT NULL,
    P_BRAND CHAR(10) NOT NULL,
    P_TYPE VARCHAR(25) NOT NULL,
    P_SIZE INTEGER NOT NULL,
    P_CONTAINER CHAR(10) NOT NULL,
    P_RETAILPRICE DECIMAL(15, 2) NOT NULL,
    P_COMMENT VARCHAR(23) NOT NULL ) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table SUPPLIER_vec ( S_SUPPKEY INTEGER NOT NULL,
    S_NAME CHAR(25) NOT NULL,
    S_ADDRESS VARCHAR(40) NOT NULL,
    S_NATIONKEY INTEGER NOT NULL,
    S_PHONE CHAR(15) NOT NULL,
    S_ACCTBAL DECIMAL(15, 2) NOT NULL,
    S_COMMENT VARCHAR(101) NOT NULL) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table PARTSUPP_vec ( PS_PARTKEY INTEGER NOT NULL,
    PS_SUPPKEY INTEGER NOT NULL,
    PS_AVAILQTY INTEGER NOT NULL,
    PS_SUPPLYCOST DECIMAL(15, 2) NOT NULL,
    PS_COMMENT VARCHAR(199) NOT NULL ) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table CUSTOMER_vec ( C_CUSTKEY INTEGER NOT NULL,
    C_NAME VARCHAR(25) NOT NULL,
    C_ADDRESS VARCHAR(40) NOT NULL,
    C_NATIONKEY INTEGER NOT NULL,
    C_PHONE CHAR(15) NOT NULL,
    C_ACCTBAL DECIMAL(15, 2) NOT NULL,
    C_MKTSEGMENT CHAR(10) NOT NULL,
    C_COMMENT VARCHAR(117) NOT NULL) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table ORDERS_vec ( O_ORDERKEY INTEGER NOT NULL,
    O_CUSTKEY INTEGER NOT NULL,
    O_ORDERSTATUS CHAR(1) NOT NULL,
    O_TOTALPRICE DECIMAL(15, 2) NOT NULL,
    O_ORDERDATE DATE NOT NULL,
    O_ORDERPRIORITY CHAR(15) NOT NULL,
    O_CLERK CHAR(15) NOT NULL,
    O_SHIPPRIORITY INTEGER NOT NULL,
    O_COMMENT VARCHAR(79) NOT NULL) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

create foreign table LINEITEM_vec ( L_ORDERKEY INTEGER NOT NULL,
    L_PARTKEY INTEGER NOT NULL,
    L_SUPPKEY INTEGER NOT NULL,
    L_LINENUMBER INTEGER NOT NULL,
    L_QUANTITY DECIMAL(15, 2) NOT NULL,
    L_EXTENDEDPRICE DECIMAL(15, 2) NOT NULL,
    L_DISCOUNT DECIMAL(15, 2) NOT NULL,
    L_TAX DECIMAL(15, 2) NOT NULL,
    L_RETURNFLAG CHAR(1) NOT NULL,
    L_LINESTATUS CHAR(1) NOT NULL,
    L_SHIPDATE DATE NOT NULL,
    L_COMMITDATE DATE NOT NULL,
    L_RECEIPTDATE DATE NOT NULL,
    L_SHIPINSTRUCT CHAR(25) NOT NULL,
    L_SHIPMODE CHAR(10) NOT NULL,
    L_COMMENT VARCHAR(44) NOT NULL) server cstore_server options (compression 'enc_lz4', block_row_count '1000', stripe_row_count '1000');

COPY customer_vec from '/home/scott/Projects/stealthdb/tpch/customer.csv' With CSV DELIMITER ',' HEADER;
COPY lineitem_vec from '/home/scott/Projects/stealthdb/tpch/lineitem.csv' With CSV DELIMITER ',' HEADER;
COPY nation_vec from '/home/scott/Projects/stealthdb/tpch/nation.csv' With CSV DELIMITER ',' HEADER;
COPY orders_vec from '/home/scott/Projects/stealthdb/tpch/orders.csv' With CSV DELIMITER ',' HEADER;
COPY part_vec from '/home/scott/Projects/stealthdb/tpch/part.csv' With CSV DELIMITER ',' HEADER;
COPY partsupp_vec from '/home/scott/Projects/stealthdb/tpch/partsupp.csv' With CSV DELIMITER ',' HEADER;
COPY region_vec from '/home/scott/Projects/stealthdb/tpch/region.csv' With CSV DELIMITER ',' HEADER;
COPY supplier_vec from '/home/scott/Projects/stealthdb/tpch/supplier.csv' With CSV DELIMITER ',' HEADER;
