<?php
// Add a ezmlm mailinglist
// $Id: ezmlm_add.php,v 1.1 2002-12-21 12:29:33 turbo Exp $
//
session_start();

require("include/pql.inc");
$_pql = new pql($USER_HOST_USR, $USER_DN, $USER_PASS);

if(!$subscribercount) {
	$subscribercount = 0;
}
if(!$killcount) {
	$killcount = 0;
}

if($domain) {
    // We're supplied with a domain, use that
	
    // check if domain exist
    if(!pql_domain_exist($_pql->ldap_linkid, $USER_SEARCH_DN_USR, $domain)){
		echo "domain &quot;$domain&quot; does not exists";
		exit();
    }
	
    // Get default domain name for this domain
    $defaultdomain = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
    $basemaildir   = pql_get_domain_value($_pql->ldap_linkid, $domain, "basemaildir");
} else {
    // We're NOT supplied with a domain, find ALL of them!
	
    // Get list of domain
    $domains = pql_get_domain_value($_pql->ldap_linkid, '*', 'administrator', "=" . $USER_DN);
    if(!is_array($domains)){
		// if no domain defined, report it
		echo "<b>Can't find any domain!</b><br>";
    } else {
		asort($domains);
		
		if(is_array($domains)){
			// Get the domainname from the domain object
			foreach($domains as $key => $domain) {
				$dont_add = 0;
				
				// Get the default domainname for the domain
				$domainname = pql_get_domain_value($_pql->ldap_linkid, $domain, "defaultdomain");
				
				// Remove duplicates
				if($domain_list) {
					for($i=0; $domain_list[$i]; $i++) {
						if($domain_list[$i] == $domainname) {
							$dont_add = 1;
						}
					}
				}
				
				if(!$dont_add) {
					$domain_list[] = $domainname;
				}
			}
		}
	}

	// Create the mailinglist creation form
	include("header.html");

?>
  <span class="title1">Create mailinglist</span>
  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
    <!-- Base configuration -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Base configuration</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List name</td>
          <td>
            <input name="listname" value="<?=$listname?>">
            <select name="domainname">
<?php
  for($i=0; $domain_list[$i]; $i++) {
?>
	          <option value="<?=$domain_list[$i]?>"><?=$domain_list[$i]?></option>
<?php
  }
?>
            </select>
          </td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">List owner</td>
          <td><input name="listowner" value="<?=$listowner?>"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Parent list</td>
          <td><input name="listparent" value="<?=$listparent?>"></td>
        </tr>
      </th>
    </table>

    <br>

    <!-- List options -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">List options</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Message <u>m</u>oderation</td>
          <td><input type="checkbox" name="moderated" accesskey="m"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><u>S</u>ubscription moderated</td>
          <td><input type="checkbox" name="submoderated" accesskey="s"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Enable <u>d</u>igest</td>
          <td><input type="checkbox" name="digest" accesskey="d"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Pre<u>f</u>ix subject</td>
          <td><input type="checkbox" name="prefix" accesskey="f"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Enable remote <u>c</u>onfiguration</td>
          <td><input type="checkbox" name="remote" accesskey="c"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><u>G</u>uarded archive</td>
          <td><input type="checkbox" name="guardedarchive" accesskey="g"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><u>A</u>rchived</td>
          <td><input type="checkbox" name="archived" accesskey="a"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><u>I</u>ndexed</td>
          <td><input type="checkbox" name="indexed" accesskey="i"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Subscriber <u>l</u>istable</td>
          <td><input type="checkbox" name="sublistable" accesskey="l"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Enable re<u>q</u>uest address</td>
          <td><input type="checkbox" name="reqaddress" accesskey="q"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Add <u>t</u>railers to messages</td>
          <td><input type="checkbox" name="trailers" accesskey="t"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Allow only s<u>u</u>bscribers posts</td>
          <td><input type="checkbox" name="subonly" accesskey="u"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><u>A</u>ccept empty subjects</td>
          <td><input type="checkbox" name="emptysubjects" accesskey="a"></td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Enable list e<u>x</u>tras</td>
          <td><input type="checkbox" name="extras" accesskey="x"></td>
        </tr>
      </th>
    </table>

    <br><!-- ======================================================= -->

    <!-- add subscribers at creation -->
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add subscriber</th>
<?php
	for($i = 1; $i <= $subscribercount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Subscriber</td>
          <td><input type="text" name="subscriber[<?=$i?>]" value="<?=$subscriber[$i]?>" onChange="this.form.submit()"></td>
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
      <th colspan="3" align="left">Add rejected address</th>
<?php
	for($i = 1; $i <= $killcount; $i++) {
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">Address</td>
          <td><input type="text" name="killlist[<?=$i?>]" value="<?=$killlist[$i]?>" onChange="this.form.submit()"></td>
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

    <input type="hidden" name="subscribercount" value="<?=$subscribercount?>">
    <input type="hidden" name="killcount" value="<?=$killcount?>">
    <input type="submit" value="save">
  </form>
<?php
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
