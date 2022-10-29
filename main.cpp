#include "crow_all.h"
#include <unistd.h>
using namespace std;
using namespace crow;

void sendFile(response &res,string filename,string contentType){
  char *cwd = get_current_dir_name();
  cout << "Current working directory: " << cwd << endl;
  free(cwd);;

  string publicFolderPath="/usr/src/cppweb/hello_crow/public/"+filename;
  ifstream in(publicFolderPath,ifstream::in);
  if( in.is_open() ){
    cout<<"-----------static content exist in public folder------------"<<endl;
    cout<<publicFolderPath<<endl;
        //  in.close();
      } else {
          perror(publicFolderPath.c_str());
      }

  if(in){
    ostringstream contents;
    contents<<in.rdbuf();
    in.close();
    res.set_header("Content-Type",contentType);
    res.write(contents.str());
  } else {
    res.code =404;
    res.write("Not Found");
  }
  res.end();
}
//-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

void sendHTML(response &res,string filename){
   sendFile(res,filename + ".html","text/html");
}

void sendImage(response &res,string filename){
   sendFile(res,"images/"+filename,"image/jpeg");
}

void sendScript(response &res,string filename){
   sendFile(res,"scripts/"+filename,"text/javascript");
}

void sendStyle(response &res,string filename){
   sendFile(res,"styles/"+filename,"text/css");
}

//---------------<-MAIN->-----------------------------------------------------------------------
int main(int argc, char* argv[]) {
  crow::SimpleApp app;

 //'STATIC CONTENT' HANDLERS
  //------sending static content(images,styles,scripts)---------
  CROW_ROUTE(app, "/styles/<string>")
    ([](const request &req,response &res, string filename){
        sendStyle(res,filename);
    });

  CROW_ROUTE(app, "/scripts/<string>")
      ([](const request &req,response &res, string filename){
          sendScript(res,filename);
      });

  CROW_ROUTE(app, "/images/<string>")
        ([](const request &req,response &res, string filename){
            sendImage(res,filename);
        });
  //----------------------------------------------------------------

  //ROUTE HANDLERS
  CROW_ROUTE(app, "/")
    ([](const request &req,response &res){
        sendHTML(res,"index");
    });

  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
