<?php
// add a domain to a bind9 ldap db
// $Id: bind9_add.php,v 2.3 2003-05-20 10:40:12 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_bind9.inc");

include("./header.html");

if($domainname) {
	$_pql_control = new pql_control($USER_HOST, $USER_DN, $USER_PASS);
}

if(($action == 'add') and ($type == 'domain')) {
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
<?php } else {
		  echo "Adding domain $domainname<br>";
		  pql_bind9_add_zone($_pql_control->ldap_linkid, $domain, $domainname);
	  }
} elseif(($action == 'add') and ($type == 'host')) {
	  if(!$hostname or !$record_type or !$dest) {
		  if(!$submit) {
			  $error_text = array();
			  $error = false;
			  
			  if(!$record_type) {
				  $error = true;
				  $error_text["record_type"] = 'Record type missing';
			  }
			  
			  if(!$hostname) {
				  $error = true;
				  $error_text["hostname"] = 'Hostname missing';
			  }

			  if(!$dest) {
				  $error = true;
				  $error_text["dest"] = 'Resource destination missing';
			  }
		  }
?>
  <span class="title1">Add a record to domain <?=$domainname?></span>

  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add host to domain
        <tr class="title">
          <td></td>
          <td>Source</td>
          <td>Type</td>
          <td>Destination</td>
        </tr>

<?php	  if($error == true) { ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"></td>
<?php		if($error_text["hostname"]) { ?>
          <td><?php echo format_error($error_text["hostname"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		}

			if($error_text["record_type"]) {
?>
          <td><?php echo format_error($error_text["record_type"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		}

			if($error_text["dest"]) {
?>
          <td><?php echo format_error($error_text["dest"]); ?></td>
<?php		} else { ?>
          <td></td>
<?php		} ?>
        </tr>

<?php	  } ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Host name</td>
          <td><input type="text" name="hostname" value="<?=$hostname?>" size="15"></td>
          <td>
            <select name="record_type">
              <option value="">Please select record type</option>
              <option value="a" <?php if($record_type == 'a') { echo "SELECTED"; } ?>>A</option>
              <option value="cname" <?php if($record_type == 'cname') { echo "SELECTED"; } ?>>CNAME</option>
              <option value="hinfo" <?php if($record_type == 'hinfo') { echo "SELECTED"; } ?>>HINFO</option>
              <option value="mx" <?php if($record_type == 'mx') { echo "SELECTED"; } ?>>MX</option>
              <option value="ns" <?php if($record_type == 'ns') { echo "SELECTED"; } ?>>NS</option>
            </select>
          </td>
          <td><input type="text" name="dest" value="<?=$dest?>" size="20"></td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="0">
    <input type="hidden" name="action" value="add">
    <input type="hidden" name="type" value="host">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="rootdn" value="<?=$rootdn?>">
    <input type="hidden" name="domainname" value="<?=$domainname?>">
    <br>
    <input type="submit" value="Save">
  </form>
<?php } else {
		  $entry["relativeDomainName"]	= $hostname;
		  $entry["zoneName"]			= $domainname;
		  $entry["dnsttl"]				= 604800;
		  switch($record_type) {
			case "a":
			  $entry["arecord"]			= $dest;
			  break;
			case "cname":
			  $entry["cnamerecord"]		= $dest;
			  break;
			case "hinfo":
			  $entry["hinforecord"]		= $dest;
			  break;
			case "mx":
			  $entry["mxrecord"]		= $dest;
			  break;
			case "ns":
			  $entry["nsrecord"]		= $dest;
			  break;
		  }

		  if(pql_bind9_add_host($_pql_control->ldap_linkid, $domain, $entry))
			$msg = "Successfully added host <u>$hostname.$domainname.</u>";
		  else
			$msg = "Failed to add $hostname to $domainname";

		  $msg = urlencode($msg);
		  $url = "domain_detail.php?rootdn=$rootdn&domain=$domain&view=dnszone&msg=$msg";

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
