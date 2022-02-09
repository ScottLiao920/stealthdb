drop extension encdb cascade;
drop table table_test;
create extension encdb;
create server cstore_server foreign data wrapper cstore_fdw;
create foreign table c_table_test(id enc_int4, comment text) server cstore_server options (compression 'lz4');
create table table_test(id enc_int4, comment text);
insert into table_test values (pg_enc_int4_encrypt(0), 'its an encrypted 0');
insert into c_table_test
select *
from table_test;
select * from c_table_test;