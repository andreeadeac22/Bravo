select concat( 'Sufficient data: ',
   case when 
      ( select COUNT(row) from points) > 100
  then 'passed' else 'failed' end );