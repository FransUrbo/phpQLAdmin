<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.21 2003-06-21 20:19:25 turbo Exp $
//
session_start();
require("./include/pql_config.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS);

// forward back to list detail page
function list_forward($domainname, $msg){
	global $domain;

    $msg = urlencode($msg);
    $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&msg=$msg&rlnb=3";
    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
}

if(!$subscribercount) {
	$subscribercount = 0;
}
if(!$killcount) {
	$killcount = 0;
}

if(!$domainname) {
	// Get list of domains
	foreach($_pql->ldap_basedn as $dn)  {
		$dom = pql_get_domain_value($_pql, $dn, 'administrator', $USER_DN);
		foreach($dom as $d) {
			$domains[] = $d;
		}
	}

    if(!is_array($domains)){
		// if no domain defined - fatal error
		die("<b>Can't find any domain!</b><br>");
    } else {
		asort($domains);
		
		if(is_array($domains)){
			// Get the domainname from the domain object
			foreach($domains as $key => $d) {
				$dont_add = 0;
				
				// Get the default domainname for the domain
				$defaultdomainname = pql_get_domain_value($_pql, $d, "defaultdomain");
				

				// Remove duplicates
				if($domain_list) {
					foreach($domain_list as $branch => $data) {
						for($i=0; $data[$i]; $i++) {
							if($data[$i] == $defaultdomainname) {
								$dont_add = 1;
							}
						}
					}
				}
				
				if(!$dont_add) {
					$domain_list[$d][] = $defaultdomainname;
				}
			}

			foreach($domains as $key => $d) {
				$dont_add = 0;
				
				// Get any additional domainname(s) for the domain
				$additionaldomainnames = pql_get_domain_value($_pql, $d, "additionaldomainname");

				if(is_array($additionaldomainnames)) {
					foreach($additionaldomainnames as $additional) {
						// Remove duplicates
						if($domain_list) {
							foreach($domain_list as $branch => $data) {
								for($i=0; $data[$i]; $i++) {
									if($data[$i] == $additional) {
										$dont_add = 1;
									}
								}
							}
						}
						
						if(!$dont_add) {
							$domain_list[$d][] = $additional;
						}
					}
				}
			}
		}
	}
}

// Default values. I think these are needed...
$checked["subhelp"]		= " CHECKED";	// -hH
$checked["subjump"]		= " CHECKED";	// -jJ
$checked["reqaddress"]	= " CHECKED";	// -qQ
$checked["subonly"]		= " CHECKED";	// -uU
$checked["prefix"]		= " CHECKED";	// -fF
$checked["archived"]	= " CHECKED";	// -aA
$checked["indexed"]		= " CHECKED";	// -iI
$checked["trailers"]	= " CHECKED";	// -tT
$checked["public"]		= " CHECKED";							// -p

if($domainname) {
	if(ereg(';', $domainname))
	  $data = split(';', $domainname);
	else {
		$data[0] = $domain;
		$data[1] = $domainname;
	}
	
	// Get basemaildir path for domain
	if(!($path = pql_get_domain_value($_pql, $data[0], "basemaildir"))) {
		die("Can't get baseMailDir path from domain '$data[1]'!");
	}
	
	require("./include/pql_ezmlm.inc");
	$ezmlm = new ezmlm($config["PQL_GLOB_EZMLM_USER"], $path);
}	

// Create list
if(isset($submit)) {
	if($listname and $domainname) {
		$ezmlm->updatelistentry(1, $listname, $data[1], $checked);
	} else {
		$error_text["listname"] = 'missing';
	}
}

require("./header.html");

if(!$domain) {
?>
  <span class="title1">Create mailinglist</span>
<?php
} else {
	$dom = pql_get_domain_value($_pql, $domain, 'ezmlmadministrator', $USER_DN);
	if(is_array($dom)) {
		if($ezmlm->mailing_lists_hostsindex["COUNT"] > $config["PQL_CONF_MAX_LISTS"][$domain]) {
?>
  <span class="title2">
    Sorry, you have reached the maximum allowed mailinglists in this domain<br>
    You have <?=$ezmlm->mailing_lists_hostsindex["COUNT"]?> mailinglists, but only <?=$config["PQL_CONF_MAX_LISTS"][$domain]?> is allowed.
  </span>
<?php
			die();
		} else { ?>
  <span class="title1">Create mailinglist in domain <?=$domain?> (<?=$domainname?>)</span>
<?php
		}
	} else {
?>
  <span class="title2">Sorry, you do not have access to create mailinglists in this domain</span>
<?php
		die();
	}
}
?>
  <br><br>

  <form action="<?=$PHP_SELF?>" method="post" name="addlist">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Base configuration</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List name</td>
          <td><?php echo format_error($error_text["listname"]); ?>

<?php
if(!$domain) {
	// No domain - select box with existing domains
?>
            <input name="listname" value="<?=$listname?>"><b>@<?php
	if(is_array($domain_list)) {
?></b>
            <select name="domainname">
<?php	foreach($domain_list as $branch => $data) {
			for($i=0; $data[$i]; $i++) {
?>
	          <option value="<?=$branch.";".$data[$i]?>"><?=$data[$i]?></option>
<?php
			}
		}
?>
            </select>
<?php
	} else {
		echo "$domain_list</b>\n";
?>
            <input type="hidden" name="domainname" value="<?=$domain_list?>">
<?php
	}
} else {
	// Got domainname, show that (and remember it!)
?>
            <input type="hidden" name="domainname" value="<?=$domainname?>">
            <input name="listname" value="<?=$listname?>"><b>@<?=$domainname?></b>
<?php
}
?>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List owner</td>
          <td>
            <input name="listowner" value="<?=$listowner?>">
            <i>(<b>optional</b>: if not mailbox in list directory)</i>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">From address</td>
          <td>
            <input name="fromaddress" value="<?=$fromaddress?>">
            <i>(<b>optional</b>: if not same as listname)</i>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Parent list</td>
          <td>
            <input name="listparent" value="<?=$listparent?>">
            <i>(<b>optional</b>: Make the list a sublist of list)</i>
          </td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th valign="top" align="left">Subscription options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subhelp" accesskey="h"<?=$onchg?><?=$checked["subhelp"]?>></td>
              <td class="title">Subscription verification</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subjump" accesskey="j"<?=$onchg?><?=$checked["subjump"]?>></td>
              <td class="title">Unsubscription verification</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="submoderated" accesskey="s"<?=$onchg?><?=$checked["submoderated"]?>></td>
              <td class="title"><u>S</u>ubscription moderation</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="sublistable" accesskey="l"<?=$onchg?><?=$checked["sublistable"]?>></td>
              <td class="title">Subscriber <u>l</u>istable</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="reqaddress" accesskey="q"<?=$onchg?><?=$checked["reqaddress"]?>></td>
              <td class="title">Enable re<u>q</u>uest address</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="subonly" accesskey="u"<?=$onchg?><?=$checked["subonly"]?>></td>
              <td class="title">Allow only s<u>u</u>bscribers posts</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="modonly" accesskey="o"<?=$onchg?><?=$checked["modonly"]?>></td>
              <td class="title">Allow only m<u>o</u>derator posts</td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left">Basic List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="moderated" accesskey="m"<?=$onchg?><?=$checked["moderated"]?>></td>
              <td class="title">Moderation - <u>M</u>essage</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="prefix" accesskey="f"<?=$onchg?><?=$checked["prefix"]?>></td>
              <td class="title">Subject pre<u>f</u>ix</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="remotecfg" accesskey="c"<?=$onchg?><?=$checked["remotecfg"]?>></td>
              <td class="title">Enable remote <u>c</u>onfiguration</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="remoteadm" accesskey="r"<?=$onchg?><?=$checked["remoteadm"]?>></td>
              <td class="title">Enable <u>r</u>emote administration</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="archived" accesskey="a"<?=$onchg?><?=$checked["archived"]?>></td>
              <td class="title"><u>A</u>rchived</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="guardedarchive" accesskey="g"<?=$onchg?><?=$guardarch?>></td>
              <td class="title"><u>G</u>uarded archive</td>
            </tr>
          </th>
        </table>
      </th>

      <th valign="top" align="left">Extra List options
        <table cellspacing="0" cellpadding="3" border="0">
          <th>
            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="digest" accesskey="d"<?=$onchg?><?=$checked["digest"]?>></td>
              <td class="title">Enable <u>d</u>igest</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="indexed" accesskey="i"<?=$onchg?><?=$checked["indexed"]?>></td>
              <td class="title"><u>I</u>ndexed</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="trailers" accesskey="t"<?=$onchg?><?=$checked["trailers"]?>></td>
              <td class="title">Add <u>t</u>railers to messages</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="checkbox" name="extras" accesskey="x"<?=$onchg?><?=$checked["extras"]?>></td>
              <td class="title">Enable list e<u>x</u>tras</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="public"<?=$checked["public"]?>></td>
              <td class="title"><u>P</u>ublic</td>
            </tr>

            <tr class="<?php table_bgcolor(); ?>">
              <td><input type="radio" name="pubpriv" accesskey="p"<?=$onchg?> value="private"<?=$checked["private"]?>></td>
              <td class="title"><u>P</u>rivate</td>
            </tr>
          </th>
        </table>
      </th>
    </table>

    <br><!-- ======================================================= -->

    <!-- add subscribers at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Subscriber address(es)</th>
<?php
	for($i = 1; $i <= $subscribercount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Subscriber</td>
          <td><input type="text" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>"<?=$onchg?>></td>
        </tr>
<?php
	}
?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?subscribercount=<?php echo ($subscribercount + 1); ?>">add <?php if($subscribercount){echo 'additional ';}?>address</a></td>
        </tr>
      </th>
    </table>
    <!-- add subscribers at creation -->

    <br><!-- ======================================================= -->

    <!-- add kill at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Rejected address(es)</th>
<?php
	for($i = 1; $i <= $killcount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Address</td>
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>"<?=$onchg?>></td>
        </tr>

<?php
	}
?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?killcount=<?php echo ($killcount + 1); ?>">add <?php if($killcount){echo 'additional ';}?>address</a></td>
        </tr>
      </th>
    </table>
    <!-- add kill at creation -->

    <br><!-- ======================================================= -->

<?php for($i = 1; $i <= $subscribercount; $i++) { ?>
    <input type="hidden" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>">
<?php } for($i = 1; $i <= $killcount; $i++) { ?>
    <input type="hidden" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>">
<?php } ?>

    <input type="submit" name="submit" value="Create list">
  </form>
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
