#include<stdio.h>
#include<string.h>
#define NUM_OF_PROD 100

struct products{
	char category[100], brand_name[50], product_name[100], colour[10], dealer[100], offer[50];
	int prod_id_num, price, stock, categorywise_compared, dealerwise_compared;/*compared variables are used while listing the products for reordering*/
	struct reviews{
		int date;
		int rating;
		char review[500];
	}r;
};

struct user_details{
	int id;
	char name[100];
	struct products purchased_products[NUM_OF_PROD];
};

void upload_prod_details(struct products prod[], int n){
	int i=0;
	printf("enter details of products\n\n");
	for(i=0; i<n; i++){
		
		char category[100], brand_name[50], product_name[100], colour[10], dealer[100], offer[50];
		int prod_id_num, price, stock; 
		
		printf("enter category\n");
		scanf("%s", category);
		strcpy(prod[i].category, category);
		fflush stdin;
		
		printf("enter product name\n");
		scanf("%s", product_name);
		strcpy(prod[i].product_name, product_name);
		fflush stdin;
		
		printf("enter product id\n");
		scanf("%d", &prod_id_num);
		prod[i].prod_id_num = prod_id_num;
		
		printf("enter brand name\n");
		scanf("%s", brand_name);
		strcpy(prod[i].brand_name, brand_name);
		fflush stdin;
		
		printf("enter price\n");
		scanf("%d", &price);
		prod[i].price = price;
		
		printf("enter colour\n");
		scanf("%s", colour);
		strcpy(prod[i].colour, colour);
		fflush stdin;
		
		printf("enter dealer\n");
		scanf("%s", dealer);
		strcpy(prod[i].dealer, dealer);
		fflush stdin;
		
		printf("enter stock quantity\n");
		scanf("%d", &stock);
		prod[i].stock = stock;
		
		printf("enter offer\n");
		scanf("%s", offer);
		strcpy(prod[i].offer, offer);
		fflush stdin;
		
		prod[i].categorywise_compared = 0;
		prod[i].dealerwise_compared = 0;
	}
}

int partition(struct products prod[], int i, int j, char category[]){
	while(i<j && strcmp(prod[i].category, category) <= 0){
		i++;
	}
	while(i<j && strcmp(prod[j].category, category) > 0){
		j--;
	}
	if(strcmp(prod[j].category, category) > 0){
		j--;
	}
	while(i<j){
		struct products temp_prod;
		temp_prod = prod[i];
		prod[i] = prod[j];
		prod[j] = temp_prod;
		while(strcmp(prod[i].category, category) <= 0){
			i++;
		}
		while(strcmp(prod[j].category, category) > 0){
			j--;
		}
	}
	return j;
}

void quick_sort(struct products prod[], int low, int high){
	if(low<high){
		int p = partition(prod, low, high, prod[low].category);
		struct products temp_prod;
		temp_prod = prod[low];
		prod[low] = prod[p];
		quick_sort(prod, low, p-1);
		quick_sort(prod, p+1, high);
	}
}

/*to sort all the matched products according to the price and stock available to decide the best seller among them */
void bubble_sort(struct products matched_products[], struct products prod[], int k, int n){
	int sorted=0, i, j;
	for(i=0; i<(k-1) && !sorted; i++){
		sorted=1;
		for(j=0; j<k-1-i; j++){
			if(matched_products[j].price > matched_products[j+1].price){
				struct products temp;
				temp = matched_products[j];
				matched_products[j] = matched_products[j+1];
				matched_products[j+1] = temp;
				
				sorted = 0;
			}
			else if(matched_products[j].price == matched_products[j+1].price && strcmp(matched_products[j].dealer, matched_products[j+1].dealer) != 0){
/*				it is assumed that the volume(total no of products sold) for a dealer depends on availability of stock 
				i.e. if less stock is available than more products sold, hence a best seller*/
				if(matched_products[j].stock > matched_products[j+1].stock){
					struct products temp;
					temp = matched_products[j];
					matched_products[j] = matched_products[j+1];
					matched_products[j+1] = temp; 
					
					sorted = 0;
				}
			}
		}
		
	}
}

int search(struct products prod[], struct products matched_products[], int n, char category[], char product_name[], char colour[], char brand_name[]){
	int i, low=0, high=n-1, k=0, l=0, buy, p, found=0;
	/*applying binary search*/
	while(low<high && !found){
		i = (low+high)/2; // i is the middle index
		if(strcmp(prod[i].category, category) == 0){
			found=1;
			int x=i;
			/*below while loop moves backward from ith index till it reaches another category*/
			while(strcmp(prod[x].category, category) == 0){
				if(
				strcmp(prod[x].product_name, product_name)==0 &&
				strcmp(prod[x].colour, colour) == 0 &&
				strcmp(prod[x].brand_name, brand_name)==0
				){
					matched_products[l++] = prod[x];
				}
				--x;
			}
			int y=i+1;
			/*below while loop moves forward from ith index till it reaches another category*/
			while(strcmp(prod[y].category, category) == 0){
				if(
				strcmp(prod[y].product_name, product_name)==0 &&
				strcmp(prod[y].colour, colour) == 0 &&
				strcmp(prod[y].brand_name, brand_name)==0
				){
					matched_products[l++] = prod[y];
				}
				y++;
			}
		}
		else if(strcmp(prod[i].category, category) < 0){
			low = i+1;
		}
		else high = i-1;
	}
	bubble_sort(matched_products, prod, l, n); /*to sort all the matched products according to the price and stock available
	                                   to decide the best seller among them */
	printf("list of best selling matched products are:\n\n");
	for(p=0; p<l; p++){
		printf("%s\n", matched_products[p].product_name);
		printf("%d\n", matched_products[p].prod_id_num);
		printf("%s\n", matched_products[p].brand_name);
		printf("%s\n", matched_products[p].category);
		printf("%s\n", matched_products[p].colour);
		printf("%s\n", matched_products[p].dealer);
		printf("%s\n", matched_products[p].offer);
		printf("%d\n", matched_products[p].price);
		printf("%d\n", matched_products[p].stock);
		printf("enter %d if you want to buy this product", p);
		printf("\n\n");
	}
	printf("enter -1 if you dont want to buy any\n");
	scanf("%d", &buy);
	return buy;
}

/*recommends products based on already purchased products. It recommends products from same category and same brand name*/
void recommend(struct products prod[], struct products purchased_products[], int n, int l){
	int i, j;
	printf("recommended products:\n");
	for(i=0; i<l; i++){/*travese through the list of purchased products*/
		for(j=0; j<n; j++){/*compares each product of purchased products with each product from total products list and recommends the matched products based on either category or brand name*/
			if(
			strcmp(prod[j].category, purchased_products[i].category)==0 ||
			strcmp(prod[j].brand_name, purchased_products[i].brand_name)==0
			){
				printf("%s\n", prod[j].product_name);
				printf("%d\n", prod[j].prod_id_num);
				printf("%s\n", prod[j].brand_name);
				printf("%s\n", prod[j].category);
				printf("%s\n", prod[j].colour);
				printf("%s\n", prod[j].dealer);
				printf("%s\n", prod[j].offer);
				printf("%d\n", prod[j].price);
				printf("%d\n", prod[j].stock);
				printf("\n\n");
			}
		}
	}
}

void categorywise_reorder_products_list(struct products prod[], int n, int reorder_quantity){
	int i, j;
	printf("products whose stock is less then reorder quantity is(category wise):\n\n");
	for(i=0; i<n; i++){
		if(!prod[i].categorywise_compared){
			printf("category %s\n\n", prod[i].category);
			for(j=i+1; j<n; j++){
				if(strcmp(prod[j].category, prod[i].category)==0){
					if(prod[j].stock < reorder_quantity){
						/*displaying that particular product details*/
						printf("%s\n", prod[j].product_name);
						printf("%d\n", prod[j].prod_id_num);
						printf("%s\n", prod[j].brand_name);
						printf("%s\n", prod[j].category);
						printf("%s\n", prod[j].colour);
						printf("%s\n", prod[j].dealer);
						printf("%s\n", prod[j].offer);
						printf("%d\n", prod[j].price);
						printf("%d\n", prod[j].stock);
						printf("\n\n");
					}
					prod[j].categorywise_compared = 1;
				}
			}	
		}	
	}
}

void dealerwise_reorder_products_list(struct products prod[], int n, int reorder_quantity){
	int i, j;
	printf("products whose stock is less then reorder quantity is(dealer wise):\n\n");
	for(i=0; i<n; i++){
		if(!prod[i].dealerwise_compared){/*it is declared as zero initially*/
			printf("dealer %s\n\n", prod[i].dealer);
			for(j=i+1; j<n; j++){
				if(strcmp(prod[j].dealer, prod[i].dealer)==0){
					if(prod[j].stock < reorder_quantity){
						/*displaying that particular product details*/
						printf("%s\n", prod[j].product_name);
						printf("%d\n", prod[j].prod_id_num);
						printf("%s\n", prod[j].brand_name);
						printf("%s\n", prod[j].category);
						printf("%s\n", prod[j].colour);
						printf("%s\n", prod[j].dealer);
						printf("%s\n", prod[j].offer);
						printf("%d\n", prod[j].price);
						printf("%d\n", prod[j].stock);
						printf("\n\n");
					}
					prod[j].dealerwise_compared = 1;
				}
			}	
		}	
	}
} 

void list_of_products(struct products prod[], int n){
	int j;
	for(j=0; j<n; j++){
		printf("%s\n", prod[j].product_name);
		printf("%d\n", prod[j].prod_id_num);
		printf("%s\n", prod[j].brand_name);
		printf("%s\n", prod[j].category);
		printf("%s\n", prod[j].colour);
		printf("%s\n", prod[j].dealer);
		printf("%s\n", prod[j].offer);
		printf("%d\n", prod[j].price);
		printf("%d\n", prod[j].stock);
		printf("\n\n");
	}
}

int main(){
	
	struct products prod[NUM_OF_PROD];
	upload_prod_details(prod, NUM_OF_PROD);
	
	quick_sort(prod, 0, NUM_OF_PROD-1);/*sorting based on category*/
	
	struct user_details user;
	int reorder_quantity=100; /*assumed that if the stock is less than 100 then its time to reorder products from the manufacturer*/
	
	printf("enter 1 to search and purchase a product\n");
	printf("enter 2 to know to the category wise products list whose stock is less than reorder quantity\n");
	printf("enter 3 to know to the dealer wise products list whose stock is less than reorder quantity\n");
	printf("enter 4 for user registration\n");
	printf("enter 5 to see all products details with available offers\n");
	int choice;
	scanf("%d", &choice);
	switch(choice){
		case 1:{
			int n, buy, l, i;/*n is no of products to search, l is to determine the actual no of purchased products*/
			l=0;
			printf("enter the number of products you want to search\n");
			scanf("%d", &n);
			struct products matched_products[NUM_OF_PROD];/*in the worst case scenario, all the products can match, thats why matched products array is of size same as total no of products*/
			for(i=0; i<n; i++){/*searches for all n products and stores purchased products in the array of purchased_products[]*/
				char category[100], product_name[100], colour[20], brand_name[50];
				printf("enter details of the product in the order of category, product name, colour and brand name\n");
				scanf("%s", category);
				scanf("%s", product_name);
				scanf("%s", colour);
				scanf("%s", brand_name);
				buy = search(prod, matched_products, NUM_OF_PROD, category, product_name, colour, brand_name);
				if(buy != -1){
					user.purchased_products[l] = matched_products[buy];
					int check_for_review;/*checks if the purchaser wants to give the review or not*/
					printf("enter 1 if you want to give the reveiw else enter 0\n");
					scanf("%d", &check_for_review);
					if(check_for_review == 1){
						printf("please enter the date(as ddmmyyyy)\n");
						int date;
						scanf("%d", &date);
						user.purchased_products[l].r.date = date;
						
						printf("enter the rating for the product in the range 0(worst) to 5(best)\n");
						int rating;
						scanf("%d", &rating);
						user.purchased_products[l].r.rating = rating;
						
						printf("please write the review(of max 500 charecters) of the product\n");
						char review[500];
						scanf("%s", review);
						strcpy(user.purchased_products[l].r.review, review);
					}
					l++;
				}
				/*if buy is -1 no product is purchased in this particular search*/
			}
			/*now user.purchased_products[] contains list of all the purchased products for the search made by the purchaser, this can be used to reccomend other products*/
			recommend(prod, user.purchased_products, NUM_OF_PROD, l);
			break;
		}
		case 2:{
			categorywise_reorder_products_list(prod, NUM_OF_PROD, reorder_quantity);
			break;
		}
		case 3:{
			dealerwise_reorder_products_list(prod, NUM_OF_PROD, reorder_quantity);
			break;
		}
			
		case 4:{
			printf("enter your id\n");
			int id;
			scanf("%d", &id);
			user.id = id;
	
			printf("enter your name\n");
			char name[100];
			scanf("%s", name);
			strcpy(user.name, name);
			break;
		}
		case 5:{
			list_of_products(prod, NUM_OF_PROD);
			break;
		}
		default:{
			printf("you havenot choosen any of the options provided\n");
			break;
		}
	}
	return 0;
}
