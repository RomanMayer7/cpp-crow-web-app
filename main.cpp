#include "crow_all.h"
#include <unistd.h>
using namespace std;
using namespace crow;

int main(int argc, char* argv[]) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")
    ([](const request &req,response &res){

      char *cwd = get_current_dir_name();
      cout << "Current working directory: " << cwd << endl;
      free(cwd);;
      string publicFolderPath="/usr/src/cppweb/hello_crow/public/index.html";
      ifstream in(publicFolderPath,ifstream::in);
      if( in.is_open() ){
        cout<<"-----------static HTML content exist in public folder------------"<<endl;
            //  in.close();
          } else {
              perror(publicFolderPath.c_str());
          }

      if(in){
        ostringstream contents;
        contents<<in.rdbuf();
        in.close();
        res.write(contents.str());
      } else {
        res.write("Not Found");
      }
      res.end();
    });

  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
