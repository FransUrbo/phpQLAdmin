<?php
// add a domain to a bind9 ldap db
// $Id: bind9_add.php,v 2.1 2003-05-19 14:34:28 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_bind9.inc");

include("./header.html");

if(!$domainname) {
?>
  <span class="title1">Create a DNS zone in branch <?=$domain?></span>

  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add domain to branch
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Domain name</td>
          <td><input type="text" name="domainname" size="40"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
  </form>
<?php
} else {
    $_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
    echo "Adding domain $domainname<br>";
    pql_bind9_add_zone($_pql_control->ldap_linkid, $domain, $domainname);
}
?>
</body>
</html>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
