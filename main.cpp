#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;
void print_function(set<string> &clauses){
	cout << "FUNCTION\nf=";
	for(auto clause : clauses){
		cout<<"("<<clause[0];
		for(int i =1;i<clause.size();++i){
			cout << "+"<<clause[i];
		}
		cout<<")";
	}
	cout << endl;
}
void print_clauses(set<string> &clauses){
	cout <<"CLAUSES"<<endl;
	for(auto clause : clauses){
		for(auto literal : clause){
			if(literal!=' ' and literal !='0')
				cout << literal << " ";
		}
		cout << endl;
	}
}

void print_matrix(vector<vector<string>> &matrix, 	map<string, vector<int>> &table){
	cout << "MATRIX"<<endl;

	for(int i = 0; i<table["U"].size();++i){
		cout << "\t"<<table["U"][i];
	}
	cout << endl;
	for(int i = 0; i<table["U"].size();++i){
		vector<string> x = matrix[i];
		cout << table["U"][i] <<"\t";
		for(auto y : x){
			cout << y<<"\t";


		}
		cout << endl;
	}
}


void get_clauses(vector<vector<string>> &matrix, set<string>&clauses){
	for(auto x : matrix){
		for(auto y : x){
			if(y!="0" and y!=""){
				clauses.insert(y);
			}
		}
	}
}
bool print_biclaster(map<string, vector<int>> &table,vector<string> &attr_names, string biclaster=""){
	int hits=0;
	cout <<endl << "\t";
	for(auto a : attr_names){
		cout <<" "<< a << "\t";
	}
	cout << endl;
	for(auto u : table["U"]){
		cout << u<<"\t";
		for(auto a: attr_names){
			if(biclaster.find(a)>=biclaster.size() and biclaster.find(to_string(u)) >= biclaster.size()){
				cout <<"["<< table[a][u-1]<<"]\t";
				++hits;
			}else{
				cout <<" "<< table[a][u-1]<<"\t";
			}
		}
		cout << endl;
	}

	return hits;
}
void print_table(map<string, vector<int>> &table,vector<string> &attr_names){
	cout <<endl<< "\t";
	for(auto a : attr_names){
		cout <<" "<< a << "\t";
	}
	cout << endl;
	for(auto u : table["U"]){
		cout << u<<"\t";
		for(auto a: attr_names){
				cout <<" "<< table[a][u-1]<<"\t";
		}
		cout << endl;
	}
}
bool replace_biclaster(map<string, vector<int>> &table,vector<string> &attr_names, string biclaster, int value){
	int hits = 0;
	cout<<endl << "\t";
	for(auto a : attr_names){
		cout <<" "<< a << "\t";
	}
	cout << endl;
	for(auto u : table["U"]){
		cout << u<<"\t";
		for(auto a: attr_names){
			if(biclaster.find(a)>=biclaster.size() and biclaster.find(to_string(u)) >= biclaster.size()){
				table[a][u-1] = value;
				cout <<"["<< table[a][u-1]<<"]\t";
				++hits;
			}else{
				cout <<" "<< table[a][u-1]<<"\t";
			}
		}
		cout << endl;
	}
	return hits;
}
void get_clauses_biclaster(map<string, vector<int>> &table,vector<string> &attr_names, set<string>&clauses, int search_for = 0){
	int* row_counters=new int[table["U"].size()];
	int* col_counters=new int[attr_names.size()];
	for(int n = 0; n<table["U"].size(); ++n){
		row_counters[n]=0;
	}
	for(int m = 0; m<attr_names.size(); ++m){
		col_counters[m]=0;
	}
	for(int c =0; c<attr_names.size();++c){
		auto col = attr_names[c];
		for(int i =0; i<table[col].size(); ++i){
			// cout << "Checking col "<<col<< " and row "<< i <<" it has value "<< table[col][i]<<endl;
			if(table[col][i]==search_for){
				col_counters[c]++;
				row_counters[i]++;
				clauses.insert(to_string(table["U"][i])+col);
			}
		}
	}
	for(int n = 0; n<table["U"].size(); ++n){
		if(row_counters[n]==attr_names.size()){//row of search_fors

			string rowname = to_string(table["U"][n]);
			cout << "wiersz "<<search_for<<" oznaczony "<<rowname<<endl;
			auto cit = clauses.begin();
			while(cit != clauses.end()){
				//cout << *cit<< " "<<cit->find(rowname)<<endl;
				if(cit->find(rowname)<cit->size()){
					clauses.erase(cit++);
				}else{
					++cit;
				}
			}
			clauses.insert(rowname);
		}
	}
	for(int m = 0; m<attr_names.size(); ++m){
		if(col_counters[m]==table["U"].size()){//col of search_fors
			string colname = attr_names[m];
			cout << "kolumna "<<search_for<<" oznaczona "<<colname<<endl;
			auto cit = clauses.begin();
			while(cit != clauses.end()){
				//cout << *cit<< " ";
				if(cit->find(colname)<cit->size()){

					clauses.erase(cit++);
				}else{
					++cit;
				}
			}
			clauses.insert(colname);
		}
	}
	delete [] row_counters;
	delete [] col_counters;
}

vector<vector<string>> get_matrix(map<string, vector<int>> &table,vector<string> &attr_names){
	vector<vector<string>> matrix;
	for(int i = 0; i<table["U"].size();++i){
		matrix.push_back({});
		for(int j=0;j<i;++j){
			matrix[i].push_back("");
		}
		for(int j = i; j<table["U"].size();++j){
			string diffs = "";
			for(int k =0; k<attr_names.size();++k){
			//	cout << "compare "<<i+1<<" with "<<j+1<<" over "<<attr_names[k]<<endl;
				if(table[attr_names[k]][i]!=table[attr_names[k]][j]){
					diffs+=attr_names[k];
				}
			}
			if(diffs=="")diffs="0";
			matrix[i].push_back(diffs);
			//cout <<  diffs<<endl;
		}
	}
	return matrix;
}
vector<vector<string>> get_matrix_modulo(map<string, vector<int>> &table,vector<string> &attr_names, string dec){
	vector<vector<string>> matrix;
	cout <<"Matrix modulo "<<dec <<endl;
	for(int i = 0; i<table["U"].size();++i){
		matrix.push_back({});
		for(int j=0;j<i;++j){
			matrix[i].push_back("");
		}
		for(int j = i; j<table["U"].size();++j){
			string diffs = "";
			if(table[dec][i]==table[dec][j]){
					diffs="0";
			}else{
				for(int k =0; k<attr_names.size();++k){
				//	cout << "compare "<<i+1<<" with "<<j+1<<" over "<<attr_names[k]<<endl;
					if(table[attr_names[k]][i]!=table[attr_names[k]][j] and attr_names[k]!=dec){
						diffs+=attr_names[k];
					}
				}
				if(diffs==""){
					diffs="0";
					cout << "WARNING: Macierz sprzeczna dla "<< i+1 <<" i " << j+1<<"!"<<endl;
				}
			}
			matrix[i].push_back(diffs);

		}
	}
	return matrix;
}


int main(){
	using namespace std;
	int objects, attributes;
	cout << "Objektów? ";
	cin >> objects;
	cout << "Atrybutów? ";
	cin >> attributes;
	map<string, vector<int>> table;
	table["U"] = {};
	cout <<"Wklej zawartość tablicy: "<<endl;
	vector<string> attr_names;
	for(int i = 0; i< attributes; ++i){
		string atr;
		cin >> atr;
		table[atr] = {};
		attr_names.push_back(atr);	
	}
	for(int i = 0; i< objects; ++i){
		int obj_name;
		cin >> obj_name;
		table["U"].push_back(obj_name);
		for(int j = 0; j<attributes;++j){
			int value;
			cin >> value;
			table[attr_names[j]].push_back(value); 
		}
	}
	// for(auto x : table){
	// 	cout << x.first<<":\t";
	// 	for(auto y : x.second){
	// 		cout << y << " \t";
	// 	}
	// 	cout <<endl;
	// }
	char decision;
	string biclast;
	bool  loop = true;
	print_table(table, attr_names);
	while(loop){

		cout << "\nCo mam zrobić?\n1. macierz odróżnialności\n2. macierz odróżnialności modulo\n3. klauzule do biklastrów na tle 0\n4. klauzule do biklastów z wybrem tła\n5. wypisz biklaster z klauzuli\n6. Wypełnij biklaster wartością\nq.  wyjście"<<endl;

		cin >> decision;
		switch(decision){
		case '1':
		{
			vector<vector<string>> matrix=get_matrix(table, attr_names);
			print_matrix(matrix, table);
			set<string> clauses;
			get_clauses(matrix, clauses);
			print_function(clauses);
			print_clauses(clauses);
		}
			break;
		case '2':
		{
			string dec;
			cout << "Który atrybut jest decyzją? [ ";
			for(auto x : attr_names){
				cout << x<<" ";
			}
			cout << "] ";
			cin >> dec;
			cout << endl;
			vector<vector<string>> matrix_dec=get_matrix_modulo(table, attr_names, "d");
			print_matrix(matrix_dec, table);
			set<string> clauses;
			get_clauses(matrix_dec, clauses);
			print_function(clauses);
			print_clauses(clauses);
		}
			break;
		case '3':
		{
			set<string> clauses;
			get_clauses_biclaster(table, attr_names, clauses);
			print_function(clauses);
			print_clauses(clauses);
		}break;
		case '4':
		{
			set<string> clauses;
			int search_for;
			cout << "Na tle czego mam szukać? ";
			cin >> search_for;
			
			get_clauses_biclaster(table, attr_names, clauses, search_for);
			print_function(clauses);
			print_clauses(clauses);
		}break;
		case '5':
		{

			cout << "Podaj klauzulę biklastra:( ! by anulować ) ";
			getline(cin, biclast);
			getline(cin, biclast);
			//cin >> biclast;
			if(biclast.find("!")<biclast.size()) break;
			if(!print_biclaster(table, attr_names, biclast)){
				cout << endl << "Klauzula koduje pusty biklaster"<<endl;
			}
		}break;
		case '6':
		{
			string temp;
			cout << "obecny biklaster ("<<biclast <<") wpisz nowy biklaser,\nlub  '-' by użyć obecnego\n( ! by anulować ) ";
			getline(cin, temp);
			getline(cin, temp);
			if(temp.find("!")<temp.size()) break;
			if(temp!="-")biclast=temp;
			cout << "Podaj warstość którą mam wypełnić biklaster: ";
			int value;
			cin >> value;
			if(!replace_biclaster(table, attr_names, biclast, value)){
				cout << endl << "Klauzula koduje pusty biklaster"<<endl;
			}

		}break;
		case 'q':
			loop = false;
			break;	
		
		default:
			cout << "zły wybór."<<endl;
			break;
	}
	}



	return 0;
}
