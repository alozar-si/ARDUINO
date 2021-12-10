"""
Requirements: create db folder
"""

import sqlite3
from sqlite3 import Error

def create_connection(db_file):
    """ create a database connection to a SQLite database """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        #print(sqlite3.version)
    except Error as e:
        print(e)
    
    return conn


def create_table(conn, create_table_sql):
    """ create a table from the create_table_sql statement
    :param conn: Connection object
    :param create_table_sql: a CREATE TABLE statement
    :return:
    """
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
        return 0
    except Error as e:
        print(e)
        return 1

def main(database, timestamp, temperature, pressure, humidity):
    tableName = "airSensor"
    sql_create_data_table = """ CREATE TABLE IF NOT EXISTS %s (
                                        id integer PRIMARY KEY,
                                        timestamp float NOT NULL,
                                        temperature float NOT NULL,
                                        pressure float NOT NULL,
                                        humidity float NOT NULL
                                    ); """ %tableName

    # create a database connection
    conn = create_connection(database)

    # create tables
    if conn is not None:
        # create projects table
        create_table(conn, sql_create_data_table)

    else:
        print("Error! cannot create the database connection.")

    with conn:
        datapoint = (timestamp, temperature, pressure, humidity)
        datapoint_id = create_datapoint(conn, tableName, datapoint)


def create_datapoint(conn, tableName, datapoint):
    """
    Create a new project into the projects table
    :param conn:
    :param tableName:
    :return: datapoint id
    """
    sql = ''' INSERT INTO %s(timestamp,temperature,pressure,humidity)
              VALUES(?,?,?,?) ''' %tableName
    cur = conn.cursor()
    cur.execute(sql, datapoint)
    conn.commit()
    return cur.lastrowid

from os.path import exists
from os import mkdir

if __name__ == '__main__':
    saveFolder = "db/"
    dbFile = "pythonsqlite.db"
    if exists(saveFolder) != 1:
        mkdir(saveFolder)
    main(saveFolder + dbFile, 1650000, 25, 1000, 50)
    main(saveFolder + dbFile, 1650000, 30, 1100, 60)