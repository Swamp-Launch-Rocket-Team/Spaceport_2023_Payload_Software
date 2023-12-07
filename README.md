# SWAMPLAUNCH

## Git Workflow Guide

This README outlines the basic Git workflow for collaborating on this project.

### Cloning the Repository
To clone this repository:

`git clone https://github.com/Swamp-Launch-Rocket-Team/Spaceport_2023_Payload_Software.git`

`cd Spaceport_2023_Payload_Software`

## Making Changes
1. Create a new branch for your work:

`git checkout -b your-branch-name`

3. Make your changes in this branch. You can check which files have changed with:

`git status`

5. Stage your changes for a commit:

`git add .`

Alternatively, add individual files using `git add <filename>`.

7. Commit your changes with a meaningful message:

`git commit -m "A brief description of changes"`

## Pushing to GitHub
1. Make sure your local branch is up to date with the main branch:

`git pull origin main`

3. Push your branch and changes to the GitHub repository:

`git push origin your-branch-name`

## Collaborating
- To incorporate the latest changes from the main branch into your branch, regularly run:

`git pull origin main`

- When your feature or fix is ready, push your branch and create a pull request on GitHub for review.

- After the review and approval, merge your branch into the main branch.

## Additional Resources
- [Git Documentation](https://git-scm.com/doc)
- [GitHub Flow Guide](https://guides.github.com/introduction/flow/)

Instructions for Use:
Replace Project Name with the name of your project.
Replace https://github.com/your-username/your-repository.git with the URL of your GitHub repository.
Update any specific commands or steps to match your project's workflow or requirements.
