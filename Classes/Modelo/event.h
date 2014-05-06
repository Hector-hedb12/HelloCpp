#ifndef  _EVENT_H_
#define  _EVENT_H_

class event{

};

class eventMove : public event{
  int pos;
  public:
  eventMove(int p){
    pos = p;
  }
};

class eventGetLife : event{
  
  
};

class eventGetBullet : event{

};

#endif
