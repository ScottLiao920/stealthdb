drop extension encdb cascade;
drop table table_test, table_tmp;
create extension encdb;
create server cstore_server foreign data wrapper cstore_fdw;
create foreign table c_table_test(id_enc_fdw enc_int4, comment text) server cstore_server options (compression 'lz4');
create table table_test(id_enc enc_int4, comment text);
insert into table_test values (pg_enc_int4_encrypt(0), 'its an encrypted 0');
insert into table_test values (pg_enc_int4_encrypt(1), 'its an encrypted 1');
insert into table_test values (pg_enc_int4_encrypt(2), 'its an encrypted 2');
create foreign table c_table_tmp(id_plain_fdw int4, comment text) server cstore_server options (compression 'lz4');
create table table_tmp(id_plain int4, comment text);
insert into table_tmp values (0, 'its a plaintext 0');
insert into table_tmp values (1, 'its a plaintext 1');
insert into table_tmp values (2, 'its a plaintext 2');
-- insert into c_table_test
-- select *
-- from table_test;
-- select * from c_table_test;