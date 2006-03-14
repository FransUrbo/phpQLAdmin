<?php
// Input form page to create a domain branch in database
// $Id: domain_add_form.php,v 1.26.2.1 2006-03-14 14:46:30 turbo Exp $
//
require("./libs/pql_session.inc");
require($_SESSION["path"]."/libs/pql_config.inc");

include($_SESSION["path"]."/header.html");

$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
$server = split(';', $_SESSION["USER_HOST"]);
$server = urldecode($server[0]);
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create domain branch in LDAP server %server%'), array('server' => $server)); ?></span>

  <br><br>

  <form action="domain_add.php" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('Create domain')?></th>
<?php if($_SESSION["BASE_DN"][1]) {
         // We have more than one Root DN (namingContexts), show
         // a drop down with every Root DN...
?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Branch base')?></td>
          <td>
            <select name="rootdn">
<?php    foreach($_SESSION["BASE_DN"] as $dn)  { ?>
              <option value="<?=$dn?>"><?=$dn?></option>
<?php    } ?>
            </select>
          </td>
        </tr>
<?php } else { ?>
        <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
<?php } ?>

        <tr>
          <td class="title"><?=$LANG->_('Branch name')?></td>
          <td>
            <input type="text" name="domain" value="<?=$_REQUEST["domain"]?>" size="30">
<?php if(! $_SESSION["BASE_DN"][1]) { ?>
            <table cellspacing="0" cellpadding="3" border="0">
              <th>
                <tr class="<?php pql_format_table(); ?>">
                  <td><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
                  <td>
<?php
	    if(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "dc") {
		// We're using a domain object
		echo $LANG->_('Using domain mode (dc separated). Don\'t use \'.\' (dots) in domain name');
	    } elseif(pql_get_define("PQL_CONF_REFERENCE_DOMAINS_WITH", $_REQUEST["rootdn"]) == "o") {
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

<?php if(pql_get_define("PQL_CONF_BIND9_USE")) { ?>
        <tr>
          <td class="title"><?=$LANG->_('Create DNS template')?></td>
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
<?php
pql_flush();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
