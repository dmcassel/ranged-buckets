# Contributing to Ranged Buckets

I welcome contributions to this example code. This document will guide you
through the process.

 - [Question or Problem?](#question)
 - [Issues and Bugs](#issue)
 - [Feature Requests](#feature)
 - [Submission Guidelines](#submit)
 
## <a name="question"></a> Got a Question or Problem?

If you have questions about the code in this repo, please file a ticket [here on
GitHub][issue tracker]. 

## <a name="issue"></a> Found an Mistake?
If you find a bug in the source code or a mistake in the documentation, you can help us by
submitting an issue to our [GitHub Issue Tracker][issue tracker]. Even better you can submit a Pull Request
with a fix for the issue you filed.

## <a name="feature"></a> Want a Feature?
You can request a new feature by submitting an issue to our [GitHub Issue Tracker][issue tracker].  If you
would like to implement a new feature then first create a new issue and discuss it with one of our
project maintainers.

## <a name="submit"></a> Submission Guidelines

### Submitting a Pull Request

#### Fork Ranged Buckets

Fork the project [on GitHub](https://github.com/marklogic/ranged-buckets/fork) and clone
your copy.

```sh
$ git clone git@github.com:username/ranged-buckets.git
$ cd ranged-buckets
$ git remote add upstream git://github.com/dmcassel/ranged-buckets.git
```

#### Create a branch for your changes

Okay, so you have decided to fix something. Create a feature branch
and start hacking:

```sh
$ git checkout -b my-feature-branch -t origin/master
```

#### Formatting code

I use [.editorconfig][] to configure our editors for proper code formatting. If you don't
use a tool that supports editorconfig be sure to configure your editor to use the settings
equivalent to our .editorconfig file.

#### Commit your changes

Make sure git knows your name and email address:

```sh
$ git config --global user.name "J. Random User"
$ git config --global user.email "j.random.user@example.com"
```

A commit message that explains your change is always good. 

#### Rebase your repo

Use `git rebase` (not `git merge`) to sync your work from time to time.

```sh
$ git fetch upstream
$ git rebase upstream/master
```


#### Test your code

I don't have any automated tests in this project, but at the very least, you
should do a search and ensure you get results that look reasonable. 

#### Push your changes

```sh
$ git push origin my-feature-branch
```

#### Submit the pull request

Go to https://github.com/username/ranged-buckets and select your feature branch. Click
the 'Pull Request' button and fill out the form.

Pull requests are usually reviewed within a few days. If you get comments
that need to be to addressed, apply your changes in a separate commit and push that to your
feature branch. Post a comment in the pull request afterwards; GitHub does
not send out notifications when you add commits to existing pull requests.

That's it! Thank you for your contribution!


#### After your pull request is merged

After your pull request is merged, you can safely delete your branch and pull the changes
from the main (upstream) repository:

* Delete the remote branch on GitHub either through the GitHub web UI or your local shell as follows:

    ```shell
    git push origin --delete my-feature-branch
    ```

* Check out the master branch:

    ```shell
    git checkout master -f
    ```

* Delete the local branch:

    ```shell
    git branch -D my-feature-branch
    ```

* Update your master with the latest upstream version:

    ```shell
    git pull --ff upstream master
    ```

[issue tracker]: https://github.com/dmcassel/ranged-buckets/issues
[.editorconfig]: http://editorconfig.org/
