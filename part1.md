# Testing

I first tested webserver and webserver_dynamic on my the local machine (that's
where I wrote the code before I uploaded it to the AWS server) using Firefox.

Firefox shows the total load time in the networking tab of developer tools.

After uploading the files to the AWS server, I tested again but with wget.

# Result

With ./webserver, I consitentlty get time to be upwards in the north of 20s but
sometimes it even exceeds 30s.

With ./webserver_dynamic, it usually takes around 5s for the complete page to
download and in my tests never exceeded 10s.