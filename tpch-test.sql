DO
$$
    BEGIN
        for i in 1..100 loop
            PERFORM SUM(c_custkey) from customer_enc;
        end loop;
    END
$$
