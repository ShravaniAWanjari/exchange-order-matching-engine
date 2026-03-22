#include "order.hpp"
#include "order_book.hpp"

int main(){
    OrderBook book;

    book.addOrder(OrderRequest{1, 100.0, 20, Side::Sell});
    book.addOrder(OrderRequest{2, 100.0, 40, Side::Sell});
    book.addOrder(OrderRequest{3, 100.0, 30, Side::Buy});
    
    book.printBook();
    

    return 0;
}