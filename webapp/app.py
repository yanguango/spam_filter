import web
import classifier

render = web.template.render('')

urls = (
    '/', 'index',
    '/check', 'check'
)
    
class index:
    def GET(self):
        return render.index()

class check:
    def POST(self):
        cat = classifier.classify(web.input()['message'])
        print(cat)
        return cat

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
