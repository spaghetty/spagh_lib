# -*- ruby -*-
require "date.rb"
require 'rubygems'
require 'redmine_client'
require 'lib/attachment'

RedmineClient::Base.configure do
  self.site = 'http://rpmbuild:delco123@www.fastdatatel.net'
  self.user = 'rpmbuild'
  self.password = 'delco123'
end

desc "Creare una nuova working directory per un repository"
task :git_create, [ :url, :repo ] do |t, args|
  sh "git clone #{args.url} #{GIT_REPO}/.git_repos/#{args.repo}/"
end

desc "Creare una nuova working directory per un repository"
task :git_link, [ :repo, :name ] do |t, args|
  sh "ln -s #{args.repo} #{GIT_REPO}/.git_repos/#{args.name}"
end

desc "Elenco dei repository git"
task :git_repos do 
  Dir.foreach("#{GIT_REPO}/.git_repos") do | x |
    if not x[0].chr=='.'
      puts x
    end
  end
end

desc "Lista degli utenti che hanno fatto dei commit"
task :git_users, [ :repo ] do |t, args|
  r = Array.new
  outp = IO.popen("git --git-dir=#{GIT_REPO}/.git_repos/#{args.repo}/.git log")
  lines = outp.readlines
  lines.each do |line| 
    if line.match(/^Author:\ (.*)\</)
      if not r.include? $1
        r.push($1)
        puts $1.strip
      end
    end
  end
end

desc "Lista dei commits di un utente"
task :git_user_commits, [ :user, :repo ] do |t, args|
  commit = ""
  author = ""
  outp = IO.popen("git --git-dir=#{GIT_REPO}/.git_repos/#{args.repo}/.git log")
  lines = outp.readlines
  lines.each do |line| 
    if line.match(/^commit\ (.*)$/)
      commit = $1.strip
      next
    end
    if line.match(/^Merge:/)
      commit = ""
      next
    end
    if line.match(/^Author:\ (.*)\</)
      author = $1.strip
      next
    end
    if line.match(/^Date:.*(\w+)\ (\w+)\ (\d+)\ (\d+:\d+:\d+)\ (\d+)/)
      d = $3
      m = $2
      y = $5
      t = $4.gsub(":", "-")
      if author == args.user and commit != ""
        (1..12).each do |i| 
          if Date::MONTHNAMES[i].match(/^#{m}/)
            m = "#{i}"
            break
          end
        end
        if d.size == 1
          d = "0" + d
        end
        if m.size == 1
          m = "0" + m
        end
        puts "%s %s-%s-%s %s" % [commit, d, m, y, t]
      end
    end
  end
end

desc "Lista dei commits di un utente"
task :git_date_commits, [ :date, :repo ] do |t, args|
  commit = ""
  author = ""
  outp = IO.popen("git --git-dir=#{GIT_REPO}/.git_repos/#{args.repo}/.git log --since=\"#{args.date}\"")
  lines = outp.readlines
  lines.each do |line| 
    if line.match(/^commit\ (.*)$/)
      commit = $1.strip
      next
    end
    if line.match(/^Merge:/)
      commit = ""
      next
    end
    if line.match(/^Author:\ (.*)\</)
      author = $1.strip
      next
    end
    if line.match(/^Date:.*(\w+)\ (\w+)\ (\d+)\ (\d+:\d+:\d+)\ (\d+)/)
      d = $3
      m = $2
      y = $5
      t = $4.gsub(":", "-")
      if not commit == "" 
        (1..12).each do |i| 
          if Date::MONTHNAMES[i].match(/^#{m}/)
            m = "#{i}"
            break
          end
        end
        if d.size == 1
          d = "0" + d
        end
        if m.size == 1
          m = "0" + m
        end
        puts "%s %s-%s-%s %s" % [commit, d, m, y, t]
      end
    end
  end
end

desc "Crea la patch di un commit"
task :git_patch, [ :commit, :name_patch, :dest_path, :repo ] do |t, args|
  sh "mkdir -p #{args.dest_path}"
  sh "rm -f *.patch"
  sh "git --git-dir=#{GIT_REPO}/.git_repos/#{args.repo}/.git format-patch -1 #{args.commit}"
  sh "mv *.patch #{args.dest_path}/#{args.name_patch}.patch"
end

desc "Massiva generazione di tutte le patch di un utente"
task :git_patch_fuser, [ :user, :dest_path, :repo ] do |t, args|
  outp = IO.popen("rake git_user_commits[#{args.user},#{args.repo}]")
  lines = outp.readlines
  lines.each do |line|
    if line.match(/^(.*)\ (.*)\ (.*)$/)
      commit = $1
      name = "%s_%s" % [$2, $3]
      sh "rake git_patch[#{commit},#{name},#{args.dest_path}/#{args.user},#{args.repo}]"
    end
  end
end

desc "Massiva generazione di tutte le patch da una data"
task :git_patch_fdate, [ :date, :dest_path, :repo ] do |t, args|
  outp = IO.popen("rake git_date_commits[#{args.date},#{args.repo}]")
  lines = outp.readlines
  lines.each do |line|
    if line.match(/^(.*)\ (.*)\ (.*)$/)
      commit = $1
      name = "%s_%s" % [$2, $3]
      sh "rake git_patch[#{commit},#{name},#{args.dest_path},#{args.repo}]"
      issues = RedmineClient::Issue.find(:all, :params => { :subject => "patch_#{commit}"})
      if issues.empty?
        sh "scp  #{args.dest_path}/#{name}.patch admin@192.168.64.1:/usr/local/src/redmine/files/"
        
        nissue = RedmineClient::Issue.new(:subject => "patch_#{commit}",
                                          :project_id => 17,
                                          :tracker_id => 4,
                                          :description => "Patch relativa al commit #{commit}, in #{name}")
        if nissue.save
          puts nissue.id
          prova = RedmineClient::Attachment.new({ :context_id => nissue.id,
                                                  :context_type => "Issue",
                                                  :filename => "#{name}.patch",
                                                  :description => "#{commit}" })
          if not prova.save
            puts prova.errors.full_messages
          end
        else
          puts nissue.errors.full_messages
        end
      end
    end
  end
end
