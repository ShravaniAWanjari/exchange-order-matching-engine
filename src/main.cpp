#include "order.hpp"
#include "order_book.hpp"

int main(){
    OrderBook book;

    book.addOrder(OrderRequest{1, 0.0, 50, Side::Buy});
    book.addOrder(OrderRequest{2, 100.0, 0, Side::Sell});
    book.addOrder(OrderRequest{3, 101.0, 25, Side::Buy});
    
    
    
    
    
    book.printBook();
    

    return 0;
}