# Server-side Code
fs = require 'fs'

PATH = 'client/static/uploads'

# Define actions which can be called from the client using ss.rpc('demo.ACTIONNAME', param1, param2...)
exports.actions = (req, res, ss) ->

  # Example of pre-loading sessions into req.session using internal middleware
  #req.use('session')

  # Uncomment line below to use the middleware defined in server/middleware/example
  #req.use('example.authenticated')

  sendMessage: (message) ->
    if message && message.length > 0            # Check for blank messages
      ss.publish.all('newMessage', message)     # Broadcast the message to everyone
      res(true)                                 # Confirm it was sent to the originating client
    else
      res(false)

  getCountAndImages: () ->
    fs.readdir PATH, (err, files) ->
      return res(err)  if err
      filetimes = (getFileTime(f) for f in files when /\.jpg$/.test f)
      filetimes.sort (a,b) -> b[1] - a[1]
      # console.log 'gcai', filetimes
      files = (f[0] for f in filetimes.slice 0, 14)
      res [filetimes.length, files]
  
getFileTime = (f) ->
  {mtime} = fs.statSync "#{PATH}/#{f}"
  [f, mtime.getTime()]
