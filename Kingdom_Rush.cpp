#include"krush.hpp"


int main(){

  string mapdir={NULL},spts={NULL},path={NULL},spt={NULL},defpt={NULL},wavdir={NULL};
  bool q=false;

  while(!q)
  {

    get_level_data(mapdir,spts,path,spt,defpt,wavdir);


    Window win(WINWIDTH,WINHEIGHT,"Kingdom Rush");
    win.draw_png(mapdir,0,0,WINWIDTH,WINHEIGHT);
    win.update_screen();


    vector<int> arrival_sec;
    vector<string> enemy_type;
    vector<bool> enemy_active;
    get_waves_data(wavdir, arrival_sec, enemy_type, enemy_active);

    vector<int> tower_x;
    vector<int> tower_y;
    vector<int> tower_type;
    initialize_towers(tower_x, tower_y, tower_type, spt);


    Event event;
    int enemy_i=0;
    int frame=0;

    int life = 15;
    int coins = 220;

    cout<<"life: "<<life<<" "<<"coins: "<<coins<<endl;

    while(true)
    {
      event=win.pollForEvent();

      if(event.type()==QUIT)
        break;
      else if(event.type()==KEY_PRESS && event.pressedKey()=='q')
      {
        q=true;
        break;
      }
      else if( (event.type()==LCLICK) )
      {
        int m_x = event.mouseX();
        int m_y = event.mouseY();
        int tower_i = spt_i( spt, spts, m_x, m_y );

        cout<<"tower "<<tower_i<<" : ";

        if( tower_i != -1 )
        {
          event=win.pollForEvent();

          while(event.type()==MMOTION || event.type()==LRELEASE || event.type() == NA)
          {
            event=win.pollForEvent();

            Delay(10);
            frame++;
            update_enemies(win, mapdir, path, defpt, enemy_active, enemy_type, arrival_sec, frame, enemy_i, life, coins);
            update_towers(win, tower_x, tower_y, tower_type);
            win.update_screen();
          }

          if( event.type()==KEY_PRESS )
          {
            char p_key = event.pressedKey();
            if( p_key =='a' || p_key=='c' || p_key=='m' || p_key=='i' )
            {
              cout<<p_key<<endl;
              set_tower( tower_type, tower_i , p_key, coins );
            }
          }
        }
      }





      update_enemies(win, mapdir, path, defpt, enemy_active, enemy_type, arrival_sec, frame, enemy_i, life, coins);
      update_towers(win, tower_x, tower_y, tower_type);
      win.update_screen();

      Delay(10);
      frame++;

    } //end level while


  } //end !q while












  return 0;
}
