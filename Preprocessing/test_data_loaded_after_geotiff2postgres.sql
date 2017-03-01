select concat( 'Load data: ',
   case when 
      ( select COUNT(row) from data) > 100
  then 'passed' else 'failed' end );