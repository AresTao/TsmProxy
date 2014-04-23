TsmProxy
========

TsmProxy is a Component of my lab project.

Its goal is to communicate to other system through http.

It both achieved http server and http client.

========================================================
composition 
===========

> xframe 
xframe is a multi-thread message-based communication framework.

it can provide asynchronous message handling.

xframe has a abstracttask class which developer can inherit. developers can achieve their own goal.

> proxy
proxy is the application that i build based on xframe. 
  >>receiver is a httpserver task which receives message from outside.
  >>sender is a httpclient task which sends message to other server.
  >>registertask publishtask subscribetask are the handling tasks which handle the message based on its message body type.


receiver and sender tests are not good at all. I will improve them.

thx!
