<?php
// add a webserver configuration to the LDAP db
// $Id: websrv_add.php,v 2.6 2004-11-05 14:53:53 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_websrv.inc");

$url["domain"] = pql_format_urls($_REQUEST["domain"]);
$url["rootdn"] = pql_format_urls($_REQUEST["rootdn"]);

include("./header.html");

if($submit) {
	$error = false;
	$error_text = array();
	
	if(!$_REQUEST["serverip"]) {
		$error = true;
		$error_text["serverip"] = $LANG->_('Missing');
	}
	
	if(!$_REQUEST["serverurl"]) {
		$error = true;
		$error_text["serverurl"] = $LANG->_('Missing');
	} elseif(!ereg('\/$', $_REQUEST["serverurl"]))
	  $_REQUEST["serverurl"] .= '/';
	
	if(!$_REQUEST["serveradmin"]) {
		$error = true;
		$error_text["serveradmin"] = $LANG->_('Missing');
	} elseif(!ereg('@', $_REQUEST["serveradmin"])) {
		$error = true;
		$error_text["serveradmin"] = $LANG->_('Invalid');
	}
	
	if(!$_REQUEST["documentroot"]) {
		$error = true;
		$error_text["documentroot"] = $LANG->_('Missing');
	} elseif(!ereg('^\/', $_REQUEST["documentroot"])) {
		$error = true;
		$error_text["documentroot"] = $LANG->_('Invalid');
	} elseif(!ereg('\/$',  $_REQUEST["documentroot"]))
	  $_REQUEST["documentroot"] .= '/';
}

if(($error == 'true') or !$_REQUEST["serverip"] or !serverurl or !$_REQUEST["serveradmin"] or !$_REQUEST["documentroot"]) {
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a webserver configuration in branch %domain%'), array('domain' => $_REQUEST["domain"])); ?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('webserver configuration'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server URL')?></td>
          <td><?php echo pql_format_error_span($error_text["serverurl"]); ?><input type="text" name="serverurl" size="40" value="<?=$_REQUEST["serverurl"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server IP[:PORT]')?></td>
          <td><?php echo pql_format_error_span($error_text["serverip"]); ?><input type="text" name="serverip" size="40" value="<?=$_REQUEST["serverip"]?>"></td>
        </tr>

<?php if(pql_get_define("PQL_CONF_BIND9_USE")) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Create DNS object')?></td>
          <td><input type="checkbox" name="dns"></td>
        </tr>

<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Server Administrator')?></td>
          <td><?php echo pql_format_error_span($error_text["serveradmin"]); ?><input type="text" name="serveradmin" size="40" value="<?=$_REQUEST["serveradmin"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Document root')?></td>
          <td><?php echo pql_format_error_span($error_text["documentroot"]); ?><input type="text" name="documentroot" size="40" value="<?=$_REQUEST["documentroot"]?>"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="submit">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rootdn" value="<?=$url["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$url["domain"]?>">
    <br>
    <input type="submit" value="Create">
  </form>
<?php
} else {
	$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_IP")]	= $_REQUEST["serverip"];
	$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_URL")]	= $_REQUEST["serverurl"];
	$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_ADMIN")]	= $_REQUEST["serveradmin"];
	$entry[pql_get_define("PQL_ATTR_WEBSRV_DOCROOT")]	= $_REQUEST["documentroot"];

	// Extract the host FQDN from the URL. A little crude...
	if(eregi('.*://', $_REQUEST["serverurl"])) {
		$fqdn = eregi_replace('.*://', '', $_REQUEST["serverurl"]);
	}
	if(eregi(':', $fqdn)) { $fqdn = eregi_replace(':.*', '', $fqdn); }
	if(eregi('/', $fqdn)) { $fqdn = eregi_replace('/.*', '', $fqdn); }

	// Add the host FQDN to the entry array.
	$entry[pql_get_define("PQL_ATTR_WEBSRV_SRV_NAME")]	= $fqdn;

	// Add the web server object
	if(pql_websrv_add_server($_pql->ldap_linkid, $_REQUEST["domain"], $entry)) {
		$msg = "Successfully added webserver configuration ".$_REQUEST["serverurl"];

		if($_REQUEST["dns"] and pql_get_define("PQL_CONF_BIND9_USE")) {
			require("./include/pql_bind9.inc");

			// Separate the domainname and hostname from the FQDN by removing the FIRST part of the FQDN.
			$tmp = split('\.', $fqdn); $domainname = ''; $hostname = $tmp[0];
			for($i=1; $tmp[$i]; $i++) {
				$domainname .= $tmp[$i];
				if($tmp[$i+1])
				  $domainname .= ".";
			}
			
			// First make sure that the zone exists.
			if(pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $domainname)) {
				$msg .= "<br>Successfully added domain $domainname";
				
				// Create a host entry
				unset($entry);
				$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($hostname);
				$entry[pql_get_define("PQL_ATTR_ZONENAME")]				= $domainname;
				$entry[pql_get_define("PQL_ATTR_DNSTTL")]				= 604800;
				$entry[pql_get_define("PQL_ATTR_ARECORD")]				= $_REQUEST["serverip"];
				
				if(pql_bind9_add_host($_pql->ldap_linkid, $_REQUEST["domain"], $entry))
				  $msg .= "<br>Successfully added host $hostname";
				else
				  $msg .= "<br>Failed to add host $hostname";
			} else
			  $msg .= "<br>Failed to add domain $domainname";
		}
	} else
	  $msg = "Failed to add webserver configuration ".$_REQUEST["serverurl"];
	
	$url =  "domain_detail.php?rootdn=".$url["rootdn"]."&domain=".$url["domain"];
	$url .= "&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
	header("Location: ".pql_get_define("PQL_CONF_URI") . "$url");
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
