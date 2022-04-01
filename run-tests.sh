 perf record -g --call-graph dwarf -F 997 -p 5951 psql tpch -f '/home/scott/Projects/stealthdb/tpch-test-EncPG.sql'
 mv perf.data ./perf-outcomes/EncPG/
 perf record -g --call-graph dwarf -F 9997 -p 5951 psql tpch -f '/home/scott/Projects/stealthdb/tpch-test-EncComp.sql' -o ./perf-outcomes/EncComp/perf.data
 mv perf.data ./perf-outcomes/EncPG/
 perf record -g --call-graph dwarf -F 997 -p 5951 psql tpch -f '/home/scott/Projects/stealthdb/tpch-test-EncVec.sql' -o ./perf-outcomes/EncVec/perf.data
 mv perf.data ./perf-outcomes/EncPG/