<?php
// Add a new mailserver to the database
// $Id: control_add_server.php,v 2.1 2002-12-29 01:04:53 turbo Exp $
//
session_start();

require("./include/pql.inc");

if(PQL_LDAP_CONTROL_USE){
    // include control api if control is used
    include("./include/pql_control.inc");
    $_pql_control = new pql_control($USER_HOST_CTR, $USER_DN, $USER_PASS);

    include("./header.html");
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
  </body>
</html>
