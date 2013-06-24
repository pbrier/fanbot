### QUICK CHAT DEMO ####

# Delete this file once you've seen how the demo works

console.log 'starting up'
ss.rpc 'demo.getCountAndImages', ([count,images]) ->
  console.log 'count', count, images
  $('#counter').text count
  for v,i in images
    $("#i#{i}").attr 'src', "/uploads/#{v}"    

# Listen out for newMessage events coming from the server
ss.event.on 'newMessage', (msg) ->

  # Example of using the Hogan Template in client/templates/chat/message.jade to generate HTML for each message
  html = ss.tmpl['chat-message'].render({message: msg, time: -> timestamp() })

  # Append it to the #chatlog div and show effect
  # $(html).hide().appendTo('#chatlog').slideDown()
  $('#chatlog').html(html)


# Show the chat form and bind to the submit action
$('#demo').on 'submit', ->

  # Grab the message from the text box
  text = $('#myMessage').val()

  # Call the 'send' funtion (below) to ensure it's valid before sending to the server
  exports.send text, (success) ->
    if success
      $('#myMessage').val('') # clear text box
    else
      alert('Oops! Unable to send message')


# Demonstrates sharing code between modules by exporting function
exports.send = (text, cb) ->
  if valid(text)
    ss.rpc('demo.sendMessage', text, cb)
  else
    cb(false)


# Private functions

timestamp = ->
  d = new Date()
  d.getHours() + ':' + pad2(d.getMinutes()) + ':' + pad2(d.getSeconds())

pad2 = (number) ->
  (if number < 10 then '0' else '') + number

valid = (text) ->
  text && text.length > 0