#ifndef  _CARD_H_
#define  _CARD_H_

/*
 * Card
 */
class card{
public:
  bool use(){}
};


class cardHeap{
  vector<card> cardPile;
  vector<card> usedCardPile;
  public:
  card get(){
    card ca = cardPile.back();
    cardPile.pop_back();
    return ca;
  }
  card push(card ca){
    usedCardPile.push_back(ca);
  }
};

#endif
