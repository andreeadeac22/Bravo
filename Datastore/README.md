Steps taken so far (after installation):

In Server PostgreSQL 9.5 (localhost:5432), right-click on Database and New Database…
Name bravodb and Owner postgres 
3. Go to Schema and click on public


Alternative: 
1-2-3 go to psql and use command CREATE DATABASE bravodb;

4. From terminal, psql, followed by \c bravodb
5. CREATE TABLE points (col integer, row integer, surface integer, thickness integer, bed integer);
6. copy points FROM '/Users/andreea/Bravo/Preprocessing/output.csv' DELIMITER ',' CSV HEADER;


7. To export database, pg_dump -U postgres bravodb > dbexport.pgsql

8. To add point column in the form  POINT(col,row):
SELECT AddGeometryColumn ('points', 'geom', 4326, 'POINT', 2);
UPDATE points SET geom= ST_SetSRID(ST_MakePoint(col, row), 4326);


SELECT ST_AsText(geom) AS geom FROM points; prints POINT in a readable way

9. Adding a auto-incrementing primary key id
ALTER TABLE points ADD COLUMN id SERIAL PRIMARY KEY;


Psql from terminal can be used to create table and copy data into it.

In case of: 

Problem 1: 
psql: FATAL:  password authentication failed for user “andreea"
Solution 1:
sudo -u postgres psql

Problem 2:
When trying to copy data to table from file in psql, 
ERROR:  relation “table_name" does not exist
Solution 2: SELECT current_database(); - check if bravodb
If not, command \c bravodb

Problem 3: 
When trying to copy data from file in psql, 
Postgres ERROR: could not open file for reading: Permission denied
Solution 3: http://stackoverflow.com/questions/19463074/postgres-error-could-not-open-file-for-reading-permission-denied

Problem 4: 
When trying to run the Visualizer, 
privilege: ERROR:  permission denied for relation data
Solution 4:
ALTER TABLE data OWNER TO user_name;
GRANT ALL PRIVILEGES ON TABLE data TO user_name;


Essential links: 
 - http://workshops.boundlessgeo.com/postgis-intro/installation.html
 
