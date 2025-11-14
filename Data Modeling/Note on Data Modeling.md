#Voloridge 
## Normalization
Prefer Denormalized since speed matters more than memory usage
### Normalized
separate, linked tables to avoid redundancy
```sql
-- Normalized version
CREATE TABLE Trader(id INT PRIMARY KEY, name TEXT);
CREATE TABLE Trade(
    id INT PRIMARY KEY,
    trader_id INT REFERENCES Trader(id),
    symbol TEXT,
    price FLOAT
);
```
Each trader’s info stored once; each trade just links via `trader_id`
### Denormalized
everything in one table for speed
```cpp
CREATE TABLE TradeFlat(
    trade_id INT,
    trader_name TEXT,
    symbol TEXT,
    price FLOAT
);
```
Faster reads (no joins) but redundant data
## Entity–Relationship Modeling(ER diagram)
## Data layout
Databases can store data `row-wise` or `column-wise`
Prefer Col wise to compute rolling AVG faster
- `Row wise`(Postgres): better for inserts/transactions
- `Column wise`(Parquet, ClickHouse): better for aggregasion, like _“avg(price)”_ across a billion rows
## ELT v.s. ETL
Pipelines that move and shape data
Prefer `ELT` bc/ we get to keep raw data and db is fast enough to transform
### ELT(Extract-> Transform-> Load)
Transform before loading into db
Received a CSV from Exchange A with weird column names
Use py to rename, clean, and filter it, then upload it to your DB
```sql
df = pd.read_csv("exchangeA.csv")
df.rename(columns={"Px": "price"}, inplace=True)
df.to_sql("clean_trades", conn)
```
### ELT(Extract->  Load-> Transform)
transform inside db
Load raw data first, then use SQL or warehouse like Snowflake, BigQuery to transform
```sql
INSERT INTO raw_trades SELECT * FROM 'exchangeA.csv';
-- Then transform inside DB
CREATE TABLE clean_trades AS
SELECT time, price, volume FROM raw_trades WHERE price > 0;
```

## Normal Form
Using functional dependencies to normalize table
`Functional Dependency`: If I know A, I can determine B
### BCNF(Boyce–Codd Normal Form)
For every FD `X → Y` in a relation,  `X` must be a superkey(can uniquely identify rows)
Every determinant is a superkey
## Correlated, Uncorrelated query
### Uncorrelated
the inner query runs **independently** of the outer query.  
It’s executed once, then its result is used by the outer query
```sql
SELECT name
FROM Trader
WHERE id IN (
    SELECT trader_id
    FROM Trade
    WHERE price > 1000
);
```
### Correlated
Inner query depends on a row of the outer query, like a tiny query executed repeatedly for each row of the outer one
```sql
SELECT name
FROM Trader t
WHERE EXISTS (
    SELECT 1
    FROM Trade tr
    WHERE tr.trader_id = t.id AND tr.price > 1000
);
```
`tr.trader_id = t.id `uses outer query’s variable `t.id`
Decorrelate by rewriting through joining on diff attributes
## Example question: 
"Design a data model to store and retrieve high-frequency trade data efficiently"
1. Data characteristics 
High-frequency trade data = time-series, append-only, very large volume  
need fast sequential writes and efficient range queries(aggregasion) by symbol and time
2. Data model
- Use a denormalized, column-oriented table
- `GROUP BY` Date (so daily queries touch only one partition)
- `SORT BY`(symbol, timestamp)(enables bin search, range scan)
- `INDEX`lightweight index on symbol, timestamp), but rely mostly on partitioning and sorted order 
3. ELT Pipeline
- transform inside db while validate to make sure `idempotent`(unique)
4. Access pattern
- pre compute say like rolling 5 min `AVG` by doing it every time we have new data first
#### Ans: 
"Use ELT-style pipeline where raw tick data is loaded first, then cleaned and transformed in db into a denormalized, col wise table partitioned by (date, symbol). It optimizes sequential scans and range queries while keeping ingestion simple and idempotent. Indexing or clustering on `(symbol, timestamp)` plus columnar compression makes high-frequency retrieval fast"

### Think like sql compiler(Relational Algebra)
Push down projection, selection etc. (greek letters) theory side

>For pipelines — mention you’d use **C++ for fast ingestion / preprocessing**, **Python for orchestration / research scripts**