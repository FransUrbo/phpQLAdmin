<?php
// add a domain to a bind9 ldap db
// $Id: bind9_add.php,v 2.11.16.3 2004-12-19 15:46:43 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_bind9.inc");

include("./header.html");

if($_REQUEST["domainname"]) {
	$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"]);
}

if(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'domain')) {
	  if(!$_REQUEST["domainname"]) {
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

    <input type="hidden" name="action" value="add">
    <input type="hidden" name="type"   value="domain">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <input type="hidden" name="rootdn" value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domain" value="<?=$_REQUEST["domain"]?>">
    <input type="submit" value="<?php echo "--&gt;&gt;"; ?>">
  </form>
<?php } else {
		  if(pql_bind9_add_zone($_pql->ldap_linkid, $_REQUEST["domain"], $_REQUEST["domainname"]))
			$msg = "Successfully added domain ".$_REQUEST["domainname"];
		  else
			$msg = "Failed to add domain ".$_REQUEST["domainname"];

		  $url = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=".$_REQUEST["view"]."&msg=".urlencode($msg);
		  header("Location: ".pql_get_define("PQL_CONF_URI") . "$url");
	  }
} elseif(($_REQUEST["action"] == 'add') and ($_REQUEST["type"] == 'host')) {
	  if(!$_REQUEST["hostname"] or !$_REQUEST["record_type"] or !$_REQUEST["dest"]) {
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
            </select>
          </td>
          <td><input type="text" name="dest" value="<?=$_REQUEST["dest"]?>" size="20"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"     value="0">
    <input type="hidden" name="action"     value="add">
    <input type="hidden" name="type"       value="host">
    <input type="hidden" name="domain"     value="<?=$_REQUEST["domain"]?>">
    <input type="hidden" name="rootdn"     value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="domainname" value="<?=$_REQUEST["domainname"]?>">
    <br>
    <input type="submit" value="Save">
  </form>
<?php } else {
		  $entry[pql_get_define("PQL_ATTR_RELATIVEDOMAINNAME")]	= pql_maybe_idna_encode($_REQUEST["hostname"]);
		  $entry[pql_get_define("PQL_ATTR_ZONENAME")]			= $_REQUEST["domainname"];
		  $entry[pql_get_define("PQL_ATTR_DNSTTL")]				= 604800;
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
		  }

		  if(pql_bind9_add_host($_pql->ldap_linkid, $_REQUEST["domain"], $entry))
			$msg = "Successfully added host <u>".$_REQUEST["hostname"].".".pql_maybe_idna_decode($_REQUEST["domainname"]).".</u>";
		  else
			$msg = "Failed to add ".$_REQUEST["hostname"]." to ".$_REQUEST["domainname"];

		  $msg = urlencode($msg);
		  $url = "domain_detail.php?rootdn=".$_REQUEST["rootdn"]."&domain=".$_REQUEST["domain"]."&view=dnszone&msg=$msg";

		  header("Location: $url");
	  }
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
