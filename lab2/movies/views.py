# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.shortcuts import render
from movies.database import Database

# Create your views here.

def show_movies(request):
    db = Database()
    db.connect()
    db.select_something()
    return render(request, 'movies/index.html', locals())
