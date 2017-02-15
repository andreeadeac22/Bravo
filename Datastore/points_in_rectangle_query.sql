SELECT col, row FROM points 
WHERE ST_MakeEnvelope(2592, 1935, 2669, 1976, 4326) && geom;