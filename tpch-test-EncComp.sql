\timing on
DO
$$
    BEGIN
        for i in 1..5 loop
                PERFORM SUM(s_suppkey) from supplier_enc;
            end loop;
    END
$$