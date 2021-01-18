#include"krush.hpp"


void get_level_data(string& mapdir,string& spts,string& path,string& spt,string& defpt,string& wavdir)
{
  cout<<"Which level do you prefer to play in? ";
  int lnum;
  cin>>lnum;

  stringstream mystr;
  mystr<<"assets/levels/"<<lnum<<"/level"<<lnum<<"-main.txt";

  ifstream lmfile;
  lmfile.open(mystr.str());
    for(int i=0;i<6;i++)
    {
      switch(i)
      {
        case 0: getline(lmfile,mapdir); break;
        case 1: getline(lmfile,spts); break;
        case 2: getline(lmfile,path); break;
        case 3: getline(lmfile,spt); break;
        case 4: getline(lmfile,defpt); break;
        case 5: getline(lmfile,wavdir); break;
      }

    }
  lmfile.close();
}


void get_waves_data(string& wavdir, vector<int>& arrival_sec,vector<string>& enemy_type, vector<bool>& enemy_active)
{
  int a_sec;
  int e_num;
  string e_type;

  ifstream wavfile;
  wavfile.open(wavdir);
    while(!wavfile.eof())
    {
      wavfile>>a_sec;

      char c;
      wavfile.get(c);
      while(c==' ')
      {
        wavfile>>e_num>>e_type;
        for(int i=0; i<e_num; i++)
        {
          arrival_sec.push_back(a_sec);
          enemy_type.push_back(e_type);
          enemy_active.push_back(false);
          //cout<<arrival_sec[arrival_sec.size()-1]<<' '<<enemy_type[enemy_type.size()-1]<<endl;
        }

        wavfile.get(c);
      }
    }
  wavfile.close();
}

void initialize_towers(vector<int>& tower_x,vector<int>& tower_y,vector<int>& tower_type,string& spt)
{
  stringstream sstream;
  sstream<<spt;

  int t_x,t_y;
  for(int i=0; i<(spt.size()/8); i++ )
  {
    sstream>>t_x>>t_y;
    tower_x.push_back(t_x);
    tower_y.push_back(t_y);
    tower_type.push_back(-1);
  }

}

string catch_enemy_x_y_dir(int& e_x, int& e_y, const int frame, const int k ,int arrival_sec, const string& enemy_type , string& path)
{
  double delta_t = (((double)frame)/100)-arrival_sec;
  int v;

  if(enemy_type == "orc")
    v=42;
  else if(enemy_type == "wolf")
    v=85;
  else if(enemy_type == "armored")
    v=21;
  else if(enemy_type == "demon")
    v=28;

  srand(time(0));
  int pxl=(v*delta_t)-(k*en_width);  //add randomness

  //////////////////////
  stringstream path_stream;
  path_stream<<path;

  int xi,yi,xf,yf;
  path_stream>>xi>>yi;
  for(int k=2; k<= (path.size()/8); k++)
  {
    path_stream>>xf>>yf;

    if( pxl< sqrt(pow((float)(xf-xi),2)+pow((float)(yf-yi),2)) )
    {
        double ratio = ( pxl/( sqrt( pow((float)(xf-xi),2) + pow((float)(yf-yi),2) ) ) );
        e_x = xi + ratio*(xf-xi) - (en_width/2);
        e_y = yi + ratio*(yf-yi) - (en_height/2);

        stringstream en_pic;
        if(abs(yf-yi) > abs(xf-xi))
        {
          if( (yf-yi)>0 )
            {
              en_pic<<"assets/enemies/"<<enemy_type<<"/front.bmp";
              return en_pic.str();
            }
          else
            {
                en_pic<<"assets/enemies/"<<enemy_type<<"/back.bmp";
                return en_pic.str();
            }
        }
        else
        {
          if( (xf-xi)>0 )
            {
              en_pic<<"assets/enemies/"<<enemy_type<<"/right.bmp";
              return en_pic.str();
            }
          else
            {
                en_pic<<"assets/enemies/"<<enemy_type<<"/left.bmp";
                return en_pic.str();
            }
        }

    }
    pxl=pxl-sqrt(pow((float)(xf-xi),2)+pow((float)(yf-yi),2));
    xi=xf; yi=yf;
  }


}

void update_enemies(Window& win,string& mapdir,string& path,string& defpt, vector<bool>& enemy_active, vector<string>& enemy_type,vector<int>& arrival_sec, int& frame,int& enemy_i, int& life, int& coins)
{
  //cout<<frame<<' '<<arrival_sec[enemy_i]*100<<endl;
  if(frame == (arrival_sec[enemy_i]*100) )
  {
    //cout<<1<<endl;
    enemy_active[enemy_i] = true;
    //cout<<frame/100<<' '<<enemy_active[enemy_i]<<endl;

    enemy_i++;
    if(arrival_sec[enemy_i] == arrival_sec[enemy_i-1])
      frame--;
  }

  int defpt_x,defpt_y;
  stringstream def;
  def<<defpt;
  def>>defpt_x>>defpt_y;

  win.draw_png(mapdir,0,0,WINWIDTH,WINHEIGHT);

  int k=0;
  for(int j=0; j<enemy_active.size(); j++)
  {
    if(enemy_active[j] == true)
    {
        int e_x=0, e_y=0;
        string str = catch_enemy_x_y_dir(e_x, e_y, frame, k, arrival_sec[j], enemy_type[j] , path);

        if( ( ( e_x + (en_width/2) ) > (defpt_x-en_width) ) && ( ( e_x + (en_width/2) ) < (defpt_x+en_width) ) && ( ( e_y + (en_height/2) ) > (defpt_y-en_height) ) && ( ( e_y + (en_height/2) ) < (defpt_y+en_height) ))
          {
            take_lives(life, enemy_type[j]);
            enemy_active[j]=false;
          }

        win.draw_bmp(str, e_x, e_y, en_width, en_height );

        if(arrival_sec[j]==arrival_sec[j+1])
          k++;
        else
          k=0;
    }
  }

}

void take_lives(int& life,string enemy_type)
{
  if( enemy_type == "orc" || enemy_type == "wolf" )
    life--;
  else if( enemy_type == "armored" || enemy_type == "demon" )
    life-=3;

  cout<<"life: "<<life<<endl;
}

int spt_i(string& spt, string& spts,int m_x,int m_y )
{
  stringstream temp;
  temp<<spts<<spt;
  int spt_width, spt_height, spt_x, spt_y;
  temp>>spt_width>>spt_height;

  for(int i=0; i < (spt.size()/8); i++ )
  {
    temp>>spt_x>>spt_y;
    if( ( m_x > ( spt_x-spt_width/2 ) ) && ( m_x < ( spt_x+spt_width/2 ) ) && ( m_y < ( spt_y+spt_height/2 ) ) && ( m_y > ( spt_y-spt_height/2 ) ) )
      return i;
  }

  return -1;
}

void set_tower(vector<int>& tower_type, int tower_i ,char p_key ,int& coins )
{

  if( p_key=='a' && coins>=70 )
  {
    tower_type[tower_i] = 0;
    coins = coins - 70;
    cout<<"coins: "<<coins<<endl;
  }
  else if( p_key=='c' && coins>=125 )
  {
    tower_type[tower_i] = 1;
    coins = coins - 125;
    cout<<"coins: "<<coins<<endl;
  }
  else if( p_key=='m' && coins>=90 )
  {
    tower_type[tower_i] = 2;
    coins = coins - 90;
    cout<<"coins: "<<coins<<endl;
  }
  else if( p_key=='i' && coins>=80 )
  {
    tower_type[tower_i] = 3;
    coins = coins - 80;
    cout<<"coins: "<<coins<<endl;
  }
  else
  {
    cout<<"SORRY! NOT ENOUGH COINS!"<<endl;
  }

}

void update_towers(Window& win,vector<int>& tower_x,vector<int>& tower_y,vector<int>& tower_type)
{
  for(int k=0; k<tower_type.size(); k++)
  {
    if( tower_type[k] != -1 )
      print_tower(win, tower_x[k], tower_y[k], tower_type[k], 63, 40);
  }
}

void print_tower(Window& win,int tower_x,int tower_y,int tower_type, int spt_width, int spt_height)
{
  stringstream tower_path;
  tower_path<<"assets/towers/";

  if(tower_type==0)
  {
    tower_path<<"archer.png";
    tower_x = tower_x - 5;
    tower_y = tower_y - 13;
  }
  else if(tower_type==1)
    tower_path<<"canon.png";
  else if(tower_type==2)
    tower_path<<"mage.png";
  else if(tower_type==3)
  {
    tower_path<<"icy.png";
    tower_x = tower_x - 5;
    tower_y = tower_y - 5;
  }

  win.draw_png( tower_path.str(), tower_x-tower_size/2, tower_y-tower_size/2-10 , tower_size, tower_size );

}
