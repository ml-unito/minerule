# How to access results

This document contains a description of how minerule results are
stored on the database.

Table "mr_query" contains a row for each successfully completed minerule. The table has the following schema:


| Field            | Type         | Null | Key | Default |
|------------------|--------------|------|-----|--------:|
| query_id         | int(10)      |      | PRI | 0       |
| query_text       | text         |      |     |         |
| query_name       | varchar(128) |      | MUL |         |
| tab_results_name | varchar(128) |      | MUL |         |
| source_tab_name  | varchar(128) |      | MUL |         |
| gal              | int(10)      |      |     | 0       |
| ral              | int(10)      |      |     | 0       |
| cal              | int(10)      |      |     | 0       |

Important fields in the table are:
- ``query_name``: contains the name of the minerule as specified in the minerule text (e.g., if the minerule reads "MINE RULE m1 AS SELECT ...", ``query_name`` will contain the string "m1" );
- ``tab_results_name``: contains the name of the table containing the minerule results;
- ``source_tab_name``: the name of the table used for mining. It may refer to the table mentioned in the ``FROM`` clause of the minerule, or to a temporary table built by the minerule;
- ``gal``: **g**roup **a**ttributes **l**ist identifier. This is a  foreign key pointing to ?
- ``ral``: **r**ule **a**ttributes **l**ist identifier. This is a foreign key pointing to ?
- ``cal``: **c**luster **a**ttributes **l**ist identifier. This is a foreign key pointing to ?


For each result created by the minerule, there should exist a table in the database having the name specified by the corresponding tab_results_name attribute. Such tables have the following schema:

| Field    | Type    | Null | Key | Default |
|----------|---------|------|-----|---------|
| bodyId   | int(11) | YES  |     | NULL    |
| headId   | int(11) | YES  |     | NULL    |
| supp     | float   | YES  |     | NULL    |
| con      | float   | YES  |     | NULL    |
| cardBody | int(11) | YES  |     | NULL    |
| cardHead | int(11) | YES  |     | NULL    |


Important fields in the table are:

- ``supp`` and ``conf``: are the support and confidence of the rule identified by the current result row;
- ``cardBody``: contains the number of elements in the body part of the rule;
- ``cardHead``: contains the number of elements in the head part of the rule;
- ``bodyId``: foreign key identifying the values belonging to the body of the rule. The name of the table containing those values can be requested to the catalogue using the following command:
			mr catalogue -n exp1 -Fr
