<?php
// add a domain to a bind9 ldap db
// $Id: bind9_add.php,v 2.25.6.2 2006-11-27 13:00:40 turbo Exp $
//
// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
require($_SESSION["path"]."/include/pql_control.inc");
require($_SESSION["path"]."/include/pql_bind9.inc");

include($_SESSION["path"]."/header.html");

if($_REQUEST["domainname"]) {
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}
// }}}

if(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'domain')) {
	  if(!$_REQUEST["domainname"]) {
		// {{{ action=add && type=domain && !domainname
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Create a DNS zone in branch %domain%'), array('domain' => $_REQUEST["domain"])); ?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('domain to branch'))); ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Domain name')?></td>
          <td><input type="text" name="domainname" size="40"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="action"          value="add">
    <input type="hidden" name="type"            value="domain">
    <input type="hidden" name="view"            value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rootdn"          value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain"          value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="dns_domain_name" value="<?=$_REQUEST["dns_domain_name"]?>">
    <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
  </form>
<?php
		// }}}
      } else {
		// {{{ Add bind9 zone
		  if(pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["domainname"]))
			$msg = "Successfully added domain ".$_REQUEST["domainname"];
		  else
			$msg = "Failed to add domain ".$_REQUEST["domainname"];

		  $url  = "domain_detail.php?rootdn=".urlencode($_REQUEST["rootdn"])."&domain=".urlencode($_REQUEST["domain"]);
		  $url .= "&dns_domain_name=".$_REQUEST["dns_domain_name"]."&view=".$_REQUEST["view"]."&msg=".urlencode($msg);

		  if(pql_get_define("PQL_CONF_DEBUG_ME"))
			die($url);
		  else
			pql_header($url);
		  // }}}
	  }
} elseif(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'host')) {
	  if(!$_REQUEST["hostname"] or !$_REQUEST["record_type"] or !$_REQUEST["dest"]) {
		  // {{{ action=add && type=host and (!hostname or !record_type or !dest)
		  if(!$_REQUEST["submit"]) {
			  $error_text = array();
			  $error = false;
			  
			  if(!$_REQUEST["record_type"]) {
				  $error = true;
				  $error_text["record_type"] = $LANG->_('Record type missing');
			  }
			  
			  if(!$_REQUEST["hostname"]) {
				  $error = true;
				  $error_text["hostname"] = $LANG->_('Hostname missing');
			  }

			  if(!$_REQUEST["dest"]) {
				  $error = true;
				  $error_text["dest"] = $LANG->_('Resource destination missing');
			  }
		  }
?>
  <span class="title1"><?php echo pql_complete_constant($LANG->_('Add a record to domain %domain%'), array('domain' => pql_maybe_idna_decode($_REQUEST["domainname"]))); ?></span>

  <br><br>

  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add host to domain
        <tr class="title">
          <td></td>
          <td>Source</td>
          <td>Type</td>
          <td>Destination</td>
        </tr>

<?php	  if($error == true) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"></td>
<?php		if($error_text["hostname"]) { ?>
          <td><?php echo pql_format_error_span($error_text["hostname"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		}

			if($error_text["record_type"]) {
?>
          <td><?php echo pql_format_error_span($error_text["record_type"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		}

			if($error_text["dest"]) {
?>
          <td><?php echo pql_format_error_span($error_text["dest"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		} ?>
        </tr>

<?php	  } ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">Host name</td>
          <td><input type="text" name="hostname" value="<?=$_REQUEST["hostname"]?>" size="15"></td>
          <td>
            <select name="record_type">
              <option value="">Please select record type</option>
              <option value="a" <?php if($_REQUEST["record_type"] == 'a') { echo "SELECTED"; } ?>>A</option>
              <option value="cname" <?php if($_REQUEST["record_type"] == 'cname') { echo "SELECTED"; } ?>>CNAME</option>
              <option value="hinfo" <?php if($_REQUEST["record_type"] == 'hinfo') { echo "SELECTED"; } ?>>HINFO</option>
              <option value="mx" <?php if($_REQUEST["record_type"] == 'mx') { echo "SELECTED"; } ?>>MX</option>
              <option value="ns" <?php if($_REQUEST["record_type"] == 'ns') { echo "SELECTED"; } ?>>NS</option>
              <option value="ptr" <?php if($_REQUEST["record_type"] == 'ptr') { echo "SELECTED"; } ?>>PTR</option>
            </select>
          </td>
          <td><input type="text" name="dest" value="<?=$_REQUEST["dest"]?>" size="20"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="4">
            <img src="images/info.png" width="16" height="16" alt="" border="0">
            <?=$LANG->_('\bNOTE\B: If destination is not IP and not within current zone, it \umust\U end with dot!')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"          value="0">
    <input type="hidden" name="action"          value="add">
    <input type="hidden" name="type"            value="host">
    <input type="hidden" name="view"            value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="domain"          value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"          value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domainname"      value="<?=$_REQUEST["domainname"]?>">
    <input type="hidden" name="dns_domain_name" value="<?=$_REQUEST["dns_domain_name"]?>">
    <br>
    <input type="submit" value="Save">
  </form>
<?php
		// }}}
	  } else {
		  // {{{ Add bind9 host
		  if($_REQUEST["record_type"] == "ptr") {
			// {{{ Special circumstances - it's a PTR.

			// Reverse the hostname ('192.168.156.1').
			$tmp  = split('\.', $_REQUEST["hostname"]);
			$count = count($tmp);
			for($i=$count-1; $i < count($tmp); $i--) {
			  $rev .= $tmp[$i];
			  if($tmp[$i-1])
				$rev .= ".";
			}
			// rev='4.156.168.192'

			// Extract the zone part from the zone/domain name ('168.192.in-addr.arpa').
			$zone = preg_replace('/\.in-addr\.arpa/', '', $_REQUEST["domainname"]);
			$zone = preg_replace('/\./', '\\\.', $zone, -1); // Just so that next regexp doesn't catch the dot.

			// Remove the zone ('.168.192') from the reverse ('4.156.168.192') => '4.156'.
			$host = preg_replace("/\.$zone/", '', $rev);

			$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= $host;
			// }}}
		  } else
			$entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($_REQUEST["hostname"]);
		  $entry[pql_get_define("PQL_ATTR_ZONENAME")]			= $_REQUEST["domainname"];
		  $entry[pql_get_define("PQL_ATTR_DNSTTL")]				= pql_bind9_get_ttl($_pql->ldap_linkid, $_REQUEST["domainname"]);
		  switch($_REQUEST["record_type"]) {
			case "a":
			  $entry[pql_get_define("PQL_ATTR_ARECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
			  break;
			case "cname":
			  $entry[pql_get_define("PQL_ATTR_CNAMERECORD")]	= pql_maybe_idna_encode($_REQUEST["dest"]);
			  break;
			case "hinfo":
			  $entry[pql_get_define("PQL_ATTR_HINFORECORD")]	= $_REQUEST["dest"];
			  break;
			case "mx":
			  $entry[pql_get_define("PQL_ATTR_MXRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
			  break;
			case "ns":
			  $entry[pql_get_define("PQL_ATTR_NSRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
			  break;
			case "ptr":
			  $entry[pql_get_define("PQL_ATTR_PTRRECORD")]		= pql_maybe_idna_encode($_REQUEST["dest"]);
			  break;
		  }

		  $dn = pql_bind9_add_host($_pql->ldap_linkid, $_REQUEST["domain"], $entry);
		  if($dn) {
			$msg = "Successfully added host <u>".$_REQUEST["hostname"].".".pql_maybe_idna_decode($_REQUEST["domainname"])."</u>";

			if(!pql_get_define("PQL_CONF_DEBUG_ME")) {
			  // We can't do this if we're debuging. The object don't exists in the db, hence
			  // we can't figure out zone name etc...
			  if(!pql_bind9_update_serial($_pql->ldap_linkid, $dn))
				die("failed to update SOA serial number");
			} else
			  echo $LANG->_("\bCan't update SOA since we're running in DEBUG_ME mode!\B");
		  } else
			$msg = "Failed to add ".$_REQUEST["hostname"]." to ".$_REQUEST["domainname"];

		  $msg = urlencode($msg);
		  $url  = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=".$_REQUEST["view"];
		  $url .= "&dns_domain_name=".$_REQUEST["dns_domain_name"]."&msg=$msg";

		  if(pql_get_define("PQL_CONF_DEBUG_ME")) {
			echo "If we wheren't debugging (file ./.DEBUG_ME exists), I'd be redirecting you to the url:<br>";
			die("<b>$url</b>");
		  } else
			pql_header($url);
		  // }}}
	  }
}
?>
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
