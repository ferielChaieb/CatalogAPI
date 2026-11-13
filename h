[1mdiff --git a/src/ProductCatalog.Api/Program.cs b/src/ProductCatalog.Api/Program.cs[m
[1mindex e1f5922..479a476 100644[m
[1m--- a/src/ProductCatalog.Api/Program.cs[m
[1m+++ b/src/ProductCatalog.Api/Program.cs[m
[36m@@ -1,40 +1,70 @@[m
 using Microsoft.EntityFrameworkCore;[m
 using ProductCatalog.Api.Data;[m
 using ProductCatalog.Api.Models;[m
[32m+[m
 var builder = WebApplication.CreateBuilder(args);[m
[32m+[m
 // Configuration de la connexion SQL Server[m
 var connectionString =[m
[31m-builder.Configuration.GetConnectionString("CatalogDbConnection");[m
[32m+[m[32m    builder.Configuration.GetConnectionString("CatalogDbConnection");[m
[32m+[m
 builder.Services.AddDbContext<AppDbContext>(options =>[m
[31m-options.UseSqlServer(connectionString));[m
[32m+[m[32m    options.UseSqlServer(connectionString));[m
[32m+[m
 // Activation d'Endpoints API Explorer & Swagger[m
 builder.Services.AddEndpointsApiExplorer();[m
 builder.Services.AddSwaggerGen();[m
[32m+[m
 var app = builder.Build();[m
[32m+[m
 // Swagger activé pour tester l'API localement et dans le conteneur[m
 app.UseSwagger();[m
 app.UseSwaggerUI();[m
[32m+[m
 // Auto-création de la base de données au démarrage[m
 using (var scope = app.Services.CreateScope())[m
 {[m
[31m-var db =[m
[31m-scope.ServiceProvider.GetRequiredService<AppDbContext>();[m
[31m-// Attend que la base soit disponible et crée le schéma[m
[31m-db.Database.EnsureCreated();[m
[32m+[m[32m    var db =[m
[32m+[m[32m        scope.ServiceProvider.GetRequiredService<AppDbContext>();[m
[32m+[m
[32m+[m[32m    // Attend que la base soit disponible et crée le schéma[m
[32m+[m[32m    db.Database.EnsureCreated();[m
 }[m
[32m+[m
 // 1. Endpoint d'observabilité / Healthcheck applicatif (Measurement- CALMS)[m
 app.MapGet("/health", () => Results.Ok(new { status = "Healthy",[m
[31m-timestamp = DateTime.UtcNow }))[m
[31m-.WithName("HealthCheck");[m
[31m-// 2. Endpoints (listing des produits et ajout d’un produit)[m
[32m+[m[32m    timestamp = DateTime.UtcNow }))[m
[32m+[m[32m    .WithName("HealthCheck");[m
[32m+[m
[32m+[m[32m// 2. Endpoints (listing des produits et ajout d'un produit)[m
 app.MapGet("/api/products", async (AppDbContext db) =>[m
[31m-await db.Products.AsNoTracking().ToListAsync())[m
[31m-.WithName("GetAllProducts");[m
[32m+[m[32m    await db.Products.AsNoTracking().ToListAsync())[m
[32m+[m[32m    .WithName("GetAllProducts");[m
[32m+[m
 app.MapPost("/api/products", async (Product product, AppDbContext[m
[31m-db) =>[m
[32m+[m[32m    db) =>[m
 {[m
[31m-db.Products.Add(product);[m
[31m-await db.SaveChangesAsync();[m
[31m-return Results.Created($"/api/products/{product.Id}", product);[m
[32m+[m[32m    db.Products.Add(product);[m
[32m+[m[32m    await db.SaveChangesAsync();[m
[32m+[m[32m    return Results.Created($"/api/products/{product.Id}", product);[m
 }).WithName("CreateProduct");[m
[32m+[m
[32m+[m[32m// 3. Endpoint : récupérer un produit par son identifiant[m
[32m+[m[32mapp.MapGet("/api/get-product/{id:int}", async (int id, AppDbContext db) =>[m
[32m+[m[32m{[m
[32m+[m[32m    var product = await db.Products.AsNoTracking()[m
[32m+[m[32m        .FirstOrDefaultAsync(p => p.Id == id);[m
[32m+[m
[32m+[m[32m    if (product is null)[m
[32m+[m[32m    {[m
[32m+[m[32m        return Results.NotFound(new[m
[32m+[m[32m        {[m
[32m+[m[32m            message = $"Aucun produit trouvé avec l'identifiant {id}."[m
[32m+[m[32m        });[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    return Results.Ok(product);[m
[32m+[m[32m})[m
[32m+[m[32m.WithName("GetProductById");[m
[32m+[m
 app.Run();[m
\ No newline at end of file[m
