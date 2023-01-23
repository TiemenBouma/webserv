#include <map>
#include <string>
#include <vector>


void init_mime_types(std::map<std::string, std::vector<std::string> > & mime_types) {

// Define the types
std::vector<std::string> extensions;

extensions.push_back("html");
extensions.push_back("htm");
extensions.push_back("shtml");
mime_types["text/html"] = extensions;

extensions.clear();
extensions.push_back("css");
mime_types["text/css"] = extensions;

extensions.clear();
extensions.push_back("xml");
extensions.push_back("rss");
mime_types["text/xml"] = extensions;

extensions.clear();
extensions.push_back("gif");
mime_types["image/gif"] = extensions;

extensions.clear();
extensions.push_back("jpeg");
extensions.push_back("jpg");
mime_types["image/jpeg"] = extensions;

extensions.clear();
extensions.push_back("js");
mime_types["application/javascript"] = extensions;

extensions.clear();
extensions.push_back("txt");
mime_types["text/plain"] = extensions;

extensions.clear();
extensions.push_back("htc");
mime_types["text/x-component"] = extensions;

extensions.clear();
extensions.push_back("mml");
mime_types["text/mathml"] = extensions;

extensions.clear();
extensions.push_back("png");
mime_types["image/png"] = extensions;

extensions.clear();
extensions.push_back("ico");
mime_types["image/x-icon"] = extensions;

extensions.clear();
extensions.push_back("jng");
mime_types["image/x-jng"] = extensions;

extensions.clear();
extensions.push_back("wbmp");
mime_types["image/vnd.wap.wbmp"] = extensions;

extensions.clear();
extensions.push_back("jar");
extensions.push_back("war");
extensions.push_back("ear");
mime_types["application/java-archive"] = extensions;

extensions.clear();
extensions.push_back("hqx");
mime_types["application/mac-binhex40"] = extensions;

extensions.clear();
extensions.push_back("pdf");
mime_types["application/pdf"] = extensions;

extensions.clear();
extensions.push_back("cco");
mime_types["application/x-cocoa"] = extensions;

extensions.clear();
extensions.push_back("jardiff");
mime_types["application/x-java-archive-diff"] = extensions;

extensions.clear();
extensions.push_back("jnlp");
mime_types["application/x-java-jnlp-file"] = extensions;

extensions.clear();
extensions.push_back("run");
mime_types["application/x-makeself"] = extensions;

extensions.clear();
extensions.push_back("pl");
extensions.push_back("pm");
mime_types["application/x-perl"] = extensions;

extensions.clear();
extensions.push_back("prc");
extensions.push_back("pdb");
mime_types["application/x-pilot"] = extensions;

extensions.clear();
extensions.push_back("rar");
mime_types["application/x-rar-compressed"] = extensions;

extensions.clear();
extensions.push_back("rpm");
mime_types["application/x-redhat-package-manager"] = extensions;

extensions.clear();
extensions.push_back("sea");
mime_types["application/x-sea"] = extensions;

extensions.clear();
extensions.push_back("swf");
mime_types["application/x-shockwave-flash"] = extensions;

extensions.clear();
extensions.push_back("sit");
mime_types["application/x-stuffit"] = extensions;

extensions.clear();
extensions.push_back("tcl");
extensions.push_back("tk");
mime_types["application/x-tcl"] = extensions;

extensions.clear();
extensions.push_back("der");
extensions.push_back("pem");
extensions.push_back("crt");
mime_types["application/x-x509-ca-cert"] = extensions;

extensions.clear();
extensions.push_back("xpi");
mime_types["application/x-xpinstall"] = extensions;

extensions.clear();
extensions.push_back("zip");
mime_types["application/zip"] = extensions;

extensions.clear();
extensions.push_back("deb");
extensions.push_back("bin");
extensions.push_back("exe");
extensions.push_back("dll");
extensions.push_back("dmg");
extensions.push_back("eot");
extensions.push_back("iso");
extensions.push_back("img");
extensions.push_back("msi");
extensions.push_back("msp");
extensions.push_back("msm");
mime_types["application/octet-stream"] = extensions;

extensions.clear();
extensions.push_back("mp3");
mime_types["audio/mpeg"] = extensions;

extensions.clear();
extensions.push_back("ra");
mime_types["audio/x-realaudio"] = extensions;

}

void init_mime_types_reverse(std::map<std::string, std::string> & mime_types_rev) {

// Define the types
mime_types_rev["html"] = "text/html";
mime_types_rev["htm"] = "text/html";
mime_types_rev["shtml"] = "text/html";
mime_types_rev["css"] = "text/css";
mime_types_rev["xml"] = "text/xml";
mime_types_rev["rss"] = "text/xml";
mime_types_rev["gif"] = "image/gif";
mime_types_rev["jpeg"] = "image/jpeg";
mime_types_rev["jpg"] = "image/jpeg";
mime_types_rev["js"] = "application/javascript";
mime_types_rev["txt"] = "text/plain";
mime_types_rev["htc"] = "text/x-component";
mime_types_rev["mml"] = "text/mathml";
mime_types_rev["png"] = "image/png";
mime_types_rev["ico"] = "image/x-icon";
mime_types_rev["jng"] = "image/x-jng";
mime_types_rev["wbmp"] = "image/vnd.wap.wbmp";
mime_types_rev["jar"] = "application/java-archive";
mime_types_rev["war"] = "application/java-archive";
mime_types_rev["ear"] = "application/java-archive";
mime_types_rev["hqx"] = "application/mac-binhex40";
mime_types_rev["pdf"] = "application/pdf";
mime_types_rev["cco"] = "application/x-cocoa";
mime_types_rev["jardiff"] = "application/x-java-archive-diff";
mime_types_rev["jnlp"] = "application/x-java-jnlp-file";
mime_types_rev["run"] = "application/x-makeself";
mime_types_rev["pl"] = "application/x-perl";
mime_types_rev["pm"] = "application/x-perl";
mime_types_rev["prc"] = "application/x-pilot";
mime_types_rev["pdb"] = "application/x-pilot";
mime_types_rev["rar"] = "application/x-rar-compressed";
mime_types_rev["rpm"] = "application/x-redhat-package-manager";
mime_types_rev["sea"] = "application/x-sea";
mime_types_rev["swf"] = "application/x-shockwave-flash";
mime_types_rev["sit"] = "application/x-stuffit";
mime_types_rev["tcl"] = "application/x-tcl";
mime_types_rev["tk"] = "application/x-tcl";
mime_types_rev["der"] = "application/x-x509-ca-cert";
mime_types_rev["pem"] = "application/x-x509-ca-cert";
mime_types_rev["crt"] = "application/x-x509-ca-cert";
mime_types_rev["xpi"] = "application/x-xpinstall";
mime_types_rev["zip"] = "application/zip";
mime_types_rev["deb"] = "application/octet-stream";
mime_types_rev["bin"] = "application/octet-stream";
mime_types_rev["exe"] = "application/octet-stream";
mime_types_rev["dll"] = "application/octet-stream";
mime_types_rev["dmg"] = "application/octet-stream";
mime_types_rev["eot"] = "application/octet-stream";
mime_types_rev["iso"] = "application/octet-stream";
mime_types_rev["img"] = "application/octet-stream";
mime_types_rev["msi"] = "application/octet-stream";
mime_types_rev["msp"] = "application/octet-stream";
mime_types_rev["msm"] = "application/octet-stream";
mime_types_rev["mp3"] = "audio/mpeg";
mime_types_rev["ra"] = "audio/x-realaudio";
}