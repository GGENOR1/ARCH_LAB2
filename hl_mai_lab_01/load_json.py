import pandas as pd
import json
from sqlalchemy import create_engine, MetaData, Table, Column, Integer, String, VARCHAR
# engine = create_engine("postgresql+psycopg2://stud:stud@localhost/archdb", echo = False)


# # Определяем схему таблицы
# metadata = MetaData()
# users_table = Table('users', metadata,
#                     Column('id', Integer, primary_key=True, autoincrement=True),
#                     Column('first_name', VARCHAR(256)),
#                     Column('last_name', VARCHAR(256)),
#                     Column('login', VARCHAR(256)),
#                     Column('password', VARCHAR(256)),
#                     Column('email', VARCHAR(256)),
#                     Column('title', VARCHAR(1024))
#                     # Добавьте остальные столбцы
#                     )

# # Создаем таблицу в базе данных
# metadata.create_all(engine)

# # Загружаем данные из JSON в DataFrame
# df = pd.read_json("hl_mai_lab_01/ExportJson.json")

# # Записываем данные из DataFrame в таблицу в базе данных
# df.to_sql('users', con=engine, if_exists='replace', index=False)
# Создаем метаданные
# Создаем метаданные
metadata = MetaData()

# Создаем таблицу пользователей
users_table = Table('users', metadata,
                    Column('id', Integer, primary_key=True),
                    Column('first_name', String(256), nullable=False),
                    Column('last_name', String(256), nullable=False),
                    Column('login', String(256), nullable=False),
                    Column('password', String(256), nullable=False),
                    Column('email', String(256)),
                    Column('title', String(1024))
                    )

# Подключение к базе данных (замените 'sqlite:///mydatabase.db' на ваше подключение)
engine = create_engine("postgresql+psycopg2://stud:stud@localhost/archdb", echo = False)

# Создаем соединение
conn = engine.connect()

# Чтение данных из JSON файла
with open('hl_mai_lab_01/ExportJson.json', 'r') as file:
    data = json.load(file)

# Вставка данных в таблицу
for record in data:
    insert_query = users_table.insert().values(
        first_name=record['first_name'],
        last_name=record['last_name'],
        login=record['login'],
        password=record['password'],
        email=record.get('email'),  # Если значение отсутствует, то передаем None
        title=record.get('title')    # Если значение отсутствует, то передаем None
        
    )
    
    conn.execute(insert_query)
    conn.commit()

# Закрываем соединение
conn.close()