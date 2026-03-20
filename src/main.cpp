#include "order.hpp"
#include "order_book.hpp"

int main(){
    OrderBook book;

    book.addOrder(OrderRequest{1,100.0,50, Side::Sell});
    book.addOrder(OrderRequest{2,99.0,40,Side::Sell});
    book.addOrder(OrderRequest{3,101.0,60,Side::Buy});

    book.printBook();

    return 0;
}