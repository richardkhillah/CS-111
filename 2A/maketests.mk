test_add:
	-./lab2_add --iterations=100 >> lab2_add.csv
	-./lab2_add --iterations=1000 >> lab2_add.csv
	-./lab2_add --iterations=10000 >> lab2_add.csv
	-./lab2_add --iterations=100000 >> lab2_add.csv

