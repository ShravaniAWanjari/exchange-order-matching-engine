#include "order.hpp"
#include "order_book.hpp"

int main(){
    OrderBook book;

    book.addOrder(OrderRequest{1,100.0,20, Side::Buy});
    book.addOrder(OrderRequest{2,100.0,40,Side::Buy});
    book.addOrder(OrderRequest{3,90.0,10,Side::Sell});

    book.printBook();

    return 0;
}