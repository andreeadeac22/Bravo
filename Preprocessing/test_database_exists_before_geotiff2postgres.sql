select concat('Database exists: ',
case 
when( SELECT count(*) FROM pg_catalog.pg_database 
	WHERE lower(datname) = lower(\'grpproj\') ) =1 
then \'passed\' else \'failed\' end );