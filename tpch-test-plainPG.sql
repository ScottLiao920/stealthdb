DO
$$
    BEGIN
        for i in 1..10 loop
            PERFORM SUM(s_suppkey) from supplier;
        end loop;
    END
$$