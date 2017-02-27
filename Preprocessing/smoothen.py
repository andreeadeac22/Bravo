
import sys
import numpy as np

from pg import DB

def delete_point(x,y):
    sql = 'DELETE * FROM data WHERE row= %d AND col = %d'
    sql = sql % (x,y)

    db.query(sql)


def insert_point(row,col,surface):
    sql = 'INSERT INTO data(id,row,col,surface) VALUES (%d,%d,%d,%f)'
    sql = sql % (_id, row, col, surface)
    
    db.query(sql)


for i in range(0,width):
    for j in range(0,height):
    if !first:
        y=surface[i,j]
        if(y-x > max_threshold)
            step_size = (y-x)/no_steps
            for step in range(x + step_size,y, step_size)
                insert_point(i,j,step)
        else if(y-x < min_threshold)
            delete_point(i,j)
