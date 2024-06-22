#!/bin/bash

if [ "$MODE" = "fg" ]; then
    exec /bin/bash
else
    /usr/local/bin/supervisord -c /etc/supervisor/conf.d/supervisord.conf
fi
