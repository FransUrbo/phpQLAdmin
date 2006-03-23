{if isset($rootdn) }
This is weird. We couldn't find the root dn for some reason. The DN we're trying to find a root DN for is: '<b>{$rootdn}</b>'.
{elsif}
This is weird. We're called{$function} with an empty DN!
{/endif}
<p>
Please report this at the <a href="http://bugs.bayour.com" target="_new">bugtracker</a>.<p>
Note that you <u><b>MUST</b></u> include exact details on how you got this error.
Every single mouse click etc - an URL won't suffice!
