<?php
// Input form page to create a domain branch in database
// $Id: domain_add_form.php,v 1.12 2003-06-25 07:06:25 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
?>
  <span class="title1">Create domain branch in LDAP server <?=$USER_HOST?></span>

  <br><br>

  <form action="domain_add.php" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=PQL_LANG_DOMAIN_ADD?></th>
<?php if($_pql->ldap_basedn[1]) {
         // We have more than one Root DN (namingContexts), show
         // a drop down with every Root DN...
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Branch base</td>
          <td>
            <select name="rootdn">
<?php    foreach($_pql->ldap_basedn as $dn)  { ?>
              <option value="<?=$dn?>"><?=$dn?></option>
<?php    } ?>
            </select>
          </td>
        </tr>

<?php } else { ?>
        <input type="hidden" name="rootdn" value="<?=$rootdn?>">
<?php } ?>
        <tr>
          <td class="title">Branch name</td>
          <td>
            <input type="text" name="domain" value="<?php echo $domain; ?>" size="26">
            <input type="submit" value="<?="--&gt;&gt;"?>">
<?php if(! $_pql->ldap_basedn[1]) { ?>
            <table cellspacing="0" cellpadding="3" border="0">
              <th>
                <tr class="<?php table_bgcolor(); ?>">
                  <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td>
<?php
	    if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		// We're using a domain object
		echo PQL_LANG_DOMAIN_ADD_INFO_DC;
	    } elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "o") {
		// We're using organization layout
		echo PQL_LANG_DOMAIN_ADD_INFO_O;
	    } else {
		// OrganizationUnit object
		echo PQL_LANG_DOMAIN_ADD_INFO_OU;
	    }
?>
                  </td>
                </tr>
              </th>
            </table>
<?php } ?>
          </td>
        </tr>
      </th>
    </table>
  </form>
  </body>
</html>
