<?php
// Input form page to create a domain branch in database
// $Id: domain_add_form.php,v 1.16 2003-11-12 14:10:05 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

include("./header.html");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create domain branch in LDAP server %server%'), array('server' => $USER_HOST)); ?></span>

  <br><br>

  <form action="domain_add.php" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Create domain')?></th>
<?php if($_pql->ldap_basedn[1]) {
         // We have more than one Root DN (namingContexts), show
         // a drop down with every Root DN...
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=$LANG->_('Branch base')?></td>
          <td>
            <select name="rootdn">
<?php    foreach($_pql->ldap_basedn as $dn)  { $dn = urldecode($dn); ?>
              <option value="<?=$dn?>"><?=$dn?></option>
<?php    } ?>
            </select>
          </td>
        </tr>
<?php } else { ?>
        <input type="hidden" name="rootdn" value="<?=$rootdn?>">
<?php } ?>

        <tr>
          <td class="title"><?=$LANG->_('Branch name')?></td>
          <td>
            <input type="text" name="domain" value="<?=$domain?>" size="30">
<?php if(! $_pql->ldap_basedn[1]) { ?>
            <table cellspacing="0" cellpadding="3" border="0">
              <th>
                <tr class="<?php table_bgcolor(); ?>">
                  <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td>
<?php
	    if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "dc") {
		// We're using a domain object
		echo $LANG->_('Using domain mode (dc separated). Don\'t use \'.\' (dots) in domain name');
	    } elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $rootdn) == "o") {
		// We're using organization layout
		echo $LANG->_('Using organization layout (o separated)');
	    } else {
		// OrganizationUnit object
		echo $LANG->_('Using organizationUnit layout (ou separated). Make sure you\'re using \'domain.tld\' for domain name');
	    }
?>
                  </td>
                </tr>
              </th>
            </table>
<?php } ?>
          </td>
        </tr>

        <tr>
          <td class="title"><?=$LANG->_('Default domain name')?></td>
          <td>
            <input type="text" name="defaultdomain" size="30">
          </td>
        </tr>

<?php if(pql_get_define("PQL_GLOB_BIND9_USE")) { ?>
        <tr>
          <td class="title"><?=$LANG->_('Create a template zone')?></td>
          <td>
            <input type="checkbox" name="template" CHECKED> Yes
          </td>
        </tr>

<?php } ?>
        <tr><td><input type="submit" value="Create"></td></tr>
      </th>
    </table>
  </form>
  </body>
</html>
