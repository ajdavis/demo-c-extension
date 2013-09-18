import demo
import mymodule

def application(environ, start_response):
    obj = mymodule.MyClass()
    outcome = demo.is_myclass(obj)

    output = 'outcome = %s\n' % outcome
    response_headers = [('Content-Length', str(len(output)))]
    start_response('200 OK', response_headers)
    return [output]
