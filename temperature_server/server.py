#!/usr/bin/env python
import SimpleHTTPServer
import SocketServer
import subprocess

class TemperatureRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/temperature':
            proc = subprocess.Popen("/usr/bin/letmecreate_examples/letmecreate_test_thermo3", stdout=subprocess.PIPE)
            temperature = proc.stdout.read()
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()

            self.wfile.write(temperature)
            self.wfile.close()
            return None

        return SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)

server = SocketServer.TCPServer(('0.0.0.0', 8080), TemperatureRequestHandler)

server.serve_forever()
