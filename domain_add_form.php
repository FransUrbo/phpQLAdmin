<?php
// Input form page to create a domain branch in database
// $Id: domain_add_form.php,v 1.4 2003-01-21 13:39:40 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");
?>
  <span class="title1">Create domain branch in LDAP server <?=$USER_HOST?></span>

  <br><br>

  <form action="domain_add.php" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=PQL_DOMAIN_ADD?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <table>
            <td class="title">Branch name</td>
            <td>
              <input type="text" name="domain" value="<?php echo $domain; ?>" size="26">
              <input type="submit" value="<?="--&gt;&gt;"?>">
              <table cellspacing="0" cellpadding="3" border="0">
                <th>
                  <tr class="<?php table_bgcolor(); ?>">
                    <td>
                      <img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                    <td>
<?php
	    if(PQL_LDAP_REFERENCE_DOMAINS_WITH == "dc") {
		// We're using a domain object
		echo PQL_DOMAIN_ADD_INFO_DC;
	    } else {
		// OrganizationUnit object
		echo PQL_DOMAIN_ADD_INFO_OU;
	    }
?>
                    </td>
                  </tr>
                </th>
              </table>
            </td>
          </table>
        </tr>
      </th>
    </table>
  </form>
  </body>
</html>
